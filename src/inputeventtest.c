#include <fcntl.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* this macro is used to tell if "bit" is set in "array"
 * it selects a byte from the array, and does a boolean AND
 * operation with a byte that only has the relevant bit set.
 * eg. to check for the 12th bit, we do (array[1] & 1<<4)
 */
#define test_bit(bit, array) (array[bit / 8] & (1 << (bit % 8)))

int
main(int argc, char **argv)
{
	int i = 0;
	char *fname = NULL;
	struct input_id ID;

	int fd;

	// find the SpaceNavigator or similar device
	fname = (char *)malloc(1000 * sizeof(char));
	while (i < 32) {
		sprintf(fname, "/dev/input/event%d", i++);
		fd = open(fname, O_RDWR | O_NONBLOCK);
		if (fd > 0) {
			ioctl(fd, EVIOCGID, &ID);

			if (ID.vendor == 0x046d &&
			    (ID.product == 0xc626 || ID.product == 0xc621 ||
			     ID.product == 0xc623 || ID.product == 0xc603)) {
				printf("Using device: %s\n", fname);
				break;
			}
		}
	}


	// detect supported features
	if (fd > 0) {
		int axes[6] = {0, 0, 0, 0, 0, 0};
		int buttons[2] = {0, 0};

		struct input_event ev;
		uint8_t evtype_bitmask[EV_MAX / 8 + 1];
		int ev_type;

		ioctl(fd, EVIOCGBIT(0, sizeof(evtype_bitmask)),
		      evtype_bitmask);

		printf("Supported event types:\n");
		for (ev_type = 0; ev_type < EV_MAX; ev_type++) {
			if (test_bit(ev_type, evtype_bitmask)) {
				/* this means that the bit is set in the event
				 * types list */
				printf("  Event type 0x%02x ", ev_type);
				switch (ev_type) {
					case EV_SYN:
						printf(" (Sync)\n");
						break;
					case EV_REL:
						printf(" (Relative Axes)\n");
						break;
					case EV_KEY:
						printf(" (Keys or "
						       "Buttons)\n");
						break;
					case EV_ABS:
						printf(" (Absolute Axes)\n");
						break;
					case EV_LED:
						printf(" (LEDs)\n");
						break;
					case EV_REP:
						printf(" (Repeat)\n");
						break;
					case EV_MSC:
						printf(" (Misc)\n");
						break;
					default:
						printf(" (Unknown event "
						       "type: 0x%04hx)\n",
						       ev_type);
				}
			}
		}

		// print out all received events
		for (;;) {
			int n = read(fd, &ev, sizeof(struct input_event));
			// printf("%d\n", n);
			if (n >= sizeof(struct input_event)) {
				switch (ev.type) {
					case EV_KEY:
						printf("Key %d pressed %d.\n",
						       ev.code, ev.value);
						buttons[ev.code] = ev.value;
						break;

					/*
					    older kernels than and
					   including 2.6.31 send EV_REL events
					   for SpaceNavigator movement newer
					   - 2.6.35 and upwards send the more
					   logical EV_ABS instead.

					    The meaning of the numbers is the
					   same. Spotted by Thomax,
					   thomax23@googlemail.com
					*/
					case EV_REL:
						printf("REL %d %d\n", ev.code,
						       ev.value);
						axes[ev.code] = ev.value;
						break;

					case EV_ABS:
						printf("ABS %d %d\n", ev.code,
						       ev.value);
						axes[ev.code] = ev.value;
						break;

					default:
						break;
				}
			}
			// printf("%d %d %d %d %d %d\n", axes[0], axes[1],
			// axes[2], axes[3], axes[4], axes[5]);
			fflush(stdout);
		}
	} else {
		fputs("Unable to open sensor!\n", stderr);
		return (-1);
	}

	close(fd);
}
