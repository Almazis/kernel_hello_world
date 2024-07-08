// SPDX-License-Identifier: GPL-2.0-only
/*
 * A hello world kernel example: Anna Vasenina <almazisdev@gmail.com>
 */

#include <linux/module.h>
#include <linux/moduleparam.h>

static char *hwm_name; 
static int hwm_hello_times = 1;

static int __init hwm_init(void)
{
	pr_info("hwm init\n");

	return 0;
}

static void __exit hwm_exit(void)
{
	kfree(hwm_name);

	pr_info("hwm exit\n");
}

static int hwm_user_name_set(const char *arg, const struct kernel_param *kp)
{
	ssize_t len = strlen(arg) + 1;

	if (hwm_name) {
		kfree(hwm_name);
		hwm_name = NULL;
	}
	
	hwm_name = kzalloc(sizeof(char) * len, GFP_KERNEL);
	if (!hwm_name)
		return -ENOMEM;
	strcpy(hwm_name, arg);
	
	return 0;
}

static int hwm_user_name_get(char *buf, const struct kernel_param *kp)
{
	ssize_t len;

	if (!hwm_name)
		return -EINVAL;
	len = strlen(hwm_name);

	strcpy(buf, hwm_name);

	return len;
}

static const struct kernel_param_ops hwm_user_name_ops = {
	.set = hwm_user_name_set,
	.get = hwm_user_name_get,
};

static int hwm_say_hello(const char *arg, const struct kernel_param *kp)
{
	int i;

	if (!hwm_name)
		return -EINVAL;

	for (i = 0; i < hwm_hello_times; i++)
		pr_warn("%d hello %s", i, hwm_name);

	return 0;
}

static const struct kernel_param_ops hwm_say_hello_ops = {
	.set = hwm_say_hello,
	.get = NULL,
};

MODULE_PARM_DESC(user_name, "User name");
module_param_cb(user_name, &hwm_user_name_ops, NULL, S_IRUGO | S_IWUSR);

MODULE_PARM_DESC(say_hello, "Say hello");
module_param_cb(say_hello, &hwm_say_hello_ops, NULL, S_IWUSR);

MODULE_PARM_DESC(hello_times, "How many times to say hello");
module_param_named(hello_times, hwm_hello_times, int, S_IRUGO | S_IWUSR);

module_init(hwm_init);
module_exit(hwm_exit);

MODULE_AUTHOR("Anna Vasenina <almazisdev@gmail.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello world module");
