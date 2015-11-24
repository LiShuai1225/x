#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#define MAX_BUFFER_SIZE       16
#define DS1307_I2C_ADDRESS    0x68
#define DS1307_I2C_DEVICE     "/dev/i2c-2"

static const unsigned char CMD = 0;

void die(char *str)
{
	fprintf(stderr, "%s, errno %d", str, errno);
	exit(1);
}

inline unsigned char dec_to_bcd(unsigned char val)
{
	return ((val / 10 * 16) + (val % 10));
}

inline unsigned char bcd_to_dec(unsigned char val)
{
	return ((val / 16 * 10) + (val % 16));
}

void set_date(int fd)
{
	unsigned char buff[MAX_BUFFER_SIZE];
	int ret;

	buff[0] = CMD;		/* command code */

	/* init date: 2015-05-01 00:00:00 */
	buff[1] = dec_to_bcd(0);	/* second */
	buff[2] = dec_to_bcd(0);	/* minute */
	buff[3] = dec_to_bcd(0);	/* hour   */
	buff[4] = dec_to_bcd(5);	/* week   */
	buff[5] = dec_to_bcd(1);	/* day    */
	buff[6] = dec_to_bcd(5);	/* month  */
	buff[7] = dec_to_bcd(15);	/* year   */
	buff[8] = 0x10;

	ret = write(fd, buff, 9);
	if (ret < 0)
		die("set date fail\n");
}

void get_date(int fd)
{
	unsigned char second, minute, hour, week, day, month, year;
	unsigned char buff[MAX_BUFFER_SIZE];
	int ret = 0;

	ret = write(fd, &CMD, 1);
	if (ret < 0)
		die("write command code fail\n");

	ret = read(fd, buff, 7);
	if (ret < 0)
		die("read date fail\n");

	second = bcd_to_dec(buff[0] & 0x7F);
	minute = bcd_to_dec(buff[1]);
	hour = bcd_to_dec(buff[2] & 0x3F);
	week = bcd_to_dec(buff[3]);
	day = bcd_to_dec(buff[4]);
	month = bcd_to_dec(buff[5]);
	year = bcd_to_dec(buff[6]);

	printf("20%02d-%02d-%02d %02d:%02d:%02d\n",
	       year, month, day, hour, minute, second);
}

int main(int argc, char *argv[])
{
	int fd, ret;

	fd = open(DS1307_I2C_DEVICE, O_RDWR);
	if (fd < 0)
		die("cannot open device " DS1307_I2C_DEVICE "\n");

	ret = ioctl(fd, I2C_TENBIT, 0);
	if (ret < 0)
		die("set address mode fail\n");

	ret = ioctl(fd, I2C_SLAVE, DS1307_I2C_ADDRESS);
	if (ret < 0)
		die("set address fail\n");

	set_date(fd);

	for (;;) {
		get_date(fd);
		sleep(1);
	}

	close(fd);

	return 0;
}
