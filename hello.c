#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

MODULE_AUTHOR("Student IO-XX");
MODULE_DESCRIPTION("Lab 4: Hello world with lists and time");
MODULE_LICENSE("Dual BSD/GPL");

// Завдання II: Параметр howmany
static uint howmany = 1;
module_param(howmany, uint, 0444);
MODULE_PARM_DESC(howmany, "Number of greetings");

// Завдання III: Структура даних
struct hello_data {
    struct list_head list;
    ktime_t time;
};

// Завдання IV: Голова списку
static LIST_HEAD(hello_list);

static int __init hello_init(void)
{
    uint i;
    struct hello_data *item;

    // Завдання II: Перевірка howmany
    if (howmany == 0) {
        pr_warn("Parameter howmany is 0\n");
        return -EINVAL;
    }
    
    if (howmany > 10) {
        pr_warn("Parameter howmany is > 10, continuing anyway...\n");
    }

    // Завдання V: Цикл
    for (i = 0; i < howmany; i++) {
        // 1. Виділення пам'яті
        item = kmalloc(sizeof(struct hello_data), GFP_KERNEL);
        if (!item) {
            pr_err("Memory allocation failed\n");
            return -ENOMEM;
        }

        // 2. Фіксація часу
        item->time = ktime_get();

        // 3. Додавання у список
        list_add_tail(&item->list, &hello_list);

        // 4. Вивід повідомлення
        pr_emerg("Hello, world!\n");
    }

    return 0;
}

static void __exit hello_exit(void)
{
    struct hello_data *item, *tmp;

    // Завдання VI: Прохід по списку та очищення
    list_for_each_entry_safe(item, tmp, &hello_list, list) {
        // Вивід часу в наносекундах
        pr_emerg("Time: %lld ns\n", ktime_to_ns(item->time));
        
        // Видалення зі списку
        list_del(&item->list);
        
        // Звільнення пам'яті
        kfree(item);
    }
}

module_init(hello_init);
module_exit(hello_exit);
