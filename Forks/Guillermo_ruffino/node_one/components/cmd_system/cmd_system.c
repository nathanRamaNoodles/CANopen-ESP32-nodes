/* Console example — various system commands

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "esp_log.h"
#include "esp_console.h"
#include "esp_system.h"
#include "esp_sleep.h"
#include "esp_spi_flash.h"
#include "driver/rtc_io.h"
#include "driver/uart.h"
#include "argtable3/argtable3.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "cmd_system.h"
#include "sdkconfig.h"


#define MAX_MOTOR_NO 3

#ifdef CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS
#define WITH_TASKS_INFO 1
#endif

static const char *TAG = "cmd_system";

static void register_free(void);
static void register_heap(void);
static void register_version(void);
static void register_restart(void);
static void register_deep_sleep(void);
static void register_light_sleep(void);
#if WITH_TASKS_INFO
static void register_tasks(void);
#endif
/* AT commands */
static void Register_AT (void);
static void Register_AT_OPEN (void);
static void Register_AT_CLOSE (void);
static void Register_AT_PUSH (void);
static void Register_AT_RELEASE (void);
static void Register_AT_CHECKSTATE (void);
static void Register_AT_CURRENT (void);
static void Register_AT_OBJECT_STATUS (void);
static void Register_AT_AUTOMODE (void);
static void Register_AT_GETSLAVEERROR (void);
static void Register_AT_SETSLAVERRORPERIOD (void); 
/*==============*/

void register_system_common(void)
{
    register_free();
    register_heap();
    register_version();
    register_restart();
#if WITH_TASKS_INFO
    register_tasks();
#endif
}

void register_system_sleep(void)
{
    register_deep_sleep();
    register_light_sleep();
}

void register_system(void)
{
    register_system_common();
    register_system_sleep();
    register_AT_commands();
}

void register_AT_commands(void) {
    Register_AT();
    Register_AT_OPEN();
    Register_AT_CLOSE();
    Register_AT_PUSH();
    Register_AT_RELEASE();
    Register_AT_CHECKSTATE();
    Register_AT_CURRENT();
    Register_AT_OBJECT_STATUS();
    Register_AT_AUTOMODE();
    Register_AT_GETSLAVEERROR();
    Register_AT_SETSLAVERRORPERIOD();
}
/*AT commands*/
static int AT (int argc, char **argv){
    printf("OK\r\n");
    return 0;
}

static int AT_OPEN (int argc, char **argv){
    printf("Opening  GIMLI . . .\r\n");
    /*do  something*/
    
    /*==============*/
    return 0;
}

static int AT_CLOSE (int argc, char **argv){
    printf("Closing  GIMLI . . .\r\n");
    /*do  something*/
    
    /*==============*/
    return 0;
}

static int AT_PUSH (int argc, char **argv){
    printf("Pushing central support . . .\r\n");
    /*do  something*/
    
    /*==============*/
    return 0;
}

static int AT_RELEASE (int argc, char **argv){
    printf("Releasing central support . . .\r\n");
    /*do  something*/
    
    /*==============*/
    return 0;
}

static int AT_CHECKSTATE (int argc, char **argv){
    /*GIMLI STATUS*/
    printf("Gimli is OPENED\r\n");
    printf("Gimli is CLSOED\r\n");
    /*CENTRAL SUPPORT STATUS*/
    printf("Central support is PUSHED\r\n");
    printf("Central support is RELEASED\r\n");
    /*do  something*/
    
    /*==============*/
    return 0;
}
static struct {
    struct arg_int *motor_num;
    struct arg_end *end;
} current_measurement_args;

static int AT_CURRENT (int argc, char **argv){

     int nerrors = arg_parse(argc, argv, (void **) &current_measurement_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, current_measurement_args.end, argv[0]);
        return 1;
    }
    if (current_measurement_args.motor_num->count) {
        uint8_t motor_no = current_measurement_args.motor_num->ival[0];
        if (motor_no == 0) {
            ESP_LOGI(TAG, "Getting current measurement of all motors\n");
            /*do  something*/
    
            /*==============*/
        }
        else if ((motor_no > 0) && (motor_no <= MAX_MOTOR_NO)) {
            ESP_LOGI(TAG, "Getting current measurement of %d motor\n", motor_no);
            /*do  something*/
    
            /*==============*/
        }
        else {
            ESP_LOGI(TAG, "Invalid motor number\n");
        }
        
    } else {
        ESP_LOGI(TAG, "Invalid motor number argument\n");
    }
    
    return 0;
}

static int AT_OBJECTSTATUS (int argc, char **argv) {
    printf("Object inside GIMLI\n");
    printf("Object is not present in GIMLI\n");
    return 0;
}

static int AT_AUTOMODE (int argc, char **argv) {
    static bool is_auto_mode_set = 0;

    if (is_auto_mode_set == 0) {
        printf("Auto mode set!\r\n");
        is_auto_mode_set = 1;
    } else if (is_auto_mode_set == 1) {
        printf("Auto mode disabled!\r\n");
         is_auto_mode_set = 0;
    }
    return 0;
}

static int AT_GETSLAVEERROR (int argc, char **argv) {
    /*do  something*/
    
    /*==============*/
    printf("SLAVE ERRORS:\n");
    return 0;
}

static struct {
    struct arg_int *time_in_ms;
    struct arg_end *end;
} error_period_ms_args;

static int AT_SETSLAVERRORPERIOD (int argc, char **argv) {
     int nerrors = arg_parse(argc, argv, (void **) &error_period_ms_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, error_period_ms_args.end, argv[0]);
        return 1;
    }
    if (error_period_ms_args.time_in_ms->count) {
        uint16_t period_ms = error_period_ms_args.time_in_ms->ival[0];
        printf("Slave will send Error status every %d ms\n", period_ms);
    } else {
        ESP_LOGI(TAG, "Invalid period\n");
    }

    return 0;
}

/*==========================AT commands init===============================*/
static void Register_AT (void) {
     const esp_console_cmd_t cmd = {
        .command = "AT",
        .help = "Get OK if communication is established",
        .hint = NULL,
        .func = &AT,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

static void Register_AT_OPEN (void) {
     const esp_console_cmd_t cmd = {
        .command = "AT+OPEN",
        .help = "Open GIMLI",
        .hint = NULL,
        .func = &AT_OPEN,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

static void Register_AT_CLOSE (void) {
    const esp_console_cmd_t cmd = {
        .command = "AT+CLOSE",
        .help = "Close GIMLI",
        .hint = NULL,
        .func = &AT_CLOSE,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

static void Register_AT_PUSH (void) {
    const esp_console_cmd_t cmd = {
        .command = "AT+PUSH",
        .help = "Push central support",
        .hint = NULL,
        .func = &AT_PUSH,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

static void Register_AT_RELEASE (void) {
    const esp_console_cmd_t cmd = {
        .command = "AT+RELEASE",
        .help = "Release central support",
        .hint = NULL,
        .func = &AT_RELEASE,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

static void Register_AT_CHECKSTATE (void) {
    const esp_console_cmd_t cmd = {
        .command = "AT+CHECKSTATE",
        .help = "Check current state of GIMLI and central support",
        .hint = NULL,
        .func = &AT_CHECKSTATE,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

static void Register_AT_CURRENT (void) {
    char *help_cmd_AT_CURRENT;
    help_cmd_AT_CURRENT = (char*) calloc(150, sizeof(char));
    sprintf(help_cmd_AT_CURRENT, "Get current measurement of selected or all motors:\n 0 - get current measurement of all motors.\n 1..%d - get current measurement of selected motor.", MAX_MOTOR_NO);

     current_measurement_args.motor_num =
        arg_int0(NULL, NULL, "<num>", "motor number");
    current_measurement_args.end = arg_end(1);
    const esp_console_cmd_t cmd = {
        .command = "AT+CURRENT",
        .help = (const char*)help_cmd_AT_CURRENT,
        .hint = NULL,
        .func = &AT_CURRENT,
        .argtable = &current_measurement_args
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

static void Register_AT_OBJECT_STATUS (void) {
    const esp_console_cmd_t cmd = {
        .command = "AT+OBJECTSTATUS",
        .help = "Get a byte from Slave device about object presence in GIMLI",
        .hint = NULL,
        .func = &AT_OBJECTSTATUS,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

static void Register_AT_AUTOMODE (void) {
    const esp_console_cmd_t cmd = {
        .command = "AT+AUTOMODE",
        .help = "Set Slave to auto/manual mode, to capture and hold object autonomous or via commands",
        .hint = NULL,
        .func = &AT_AUTOMODE,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

static void Register_AT_GETSLAVEERROR (void) {
    const esp_console_cmd_t cmd = {
        .command = "AT+GETSLAVEERROR",
        .help = "Get Error status of Slave device",
        .hint = NULL,
        .func = &AT_GETSLAVEERROR,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

static void Register_AT_SETSLAVERRORPERIOD (void) {
    error_period_ms_args.time_in_ms =
        arg_int0(NULL, NULL, "<t>", "period in ms");
        error_period_ms_args.end = arg_end(1);
    const esp_console_cmd_t cmd = {
        .command = "AT+GETSLAVEERRORPERIOD",
        .help = "Get Error status of Slave device in set period",
        .hint = NULL,
        .func = &AT_SETSLAVERRORPERIOD,
        .argtable = &error_period_ms_args
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}
/* 'version' command */
static int get_version(int argc, char **argv)
{
    esp_chip_info_t info;
    esp_chip_info(&info);
    printf("IDF Version:%s\r\n", esp_get_idf_version());
    printf("Chip info:\r\n");
    printf("\tmodel:%s\r\n", info.model == CHIP_ESP32 ? "ESP32" : "Unknow");
    printf("\tcores:%d\r\n", info.cores);
    printf("\tfeature:%s%s%s%s%d%s\r\n",
           info.features & CHIP_FEATURE_WIFI_BGN ? "/802.11bgn" : "",
           info.features & CHIP_FEATURE_BLE ? "/BLE" : "",
           info.features & CHIP_FEATURE_BT ? "/BT" : "",
           info.features & CHIP_FEATURE_EMB_FLASH ? "/Embedded-Flash:" : "/External-Flash:",
           spi_flash_get_chip_size() / (1024 * 1024), " MB");
    printf("\trevision number:%d\r\n", info.revision);
    return 0;
}

static void register_version(void)
{
    const esp_console_cmd_t cmd = {
        .command = "version",
        .help = "Get version of chip and SDK",
        .hint = NULL,
        .func = &get_version,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

/** 'restart' command restarts the program */

static int restart(int argc, char **argv)
{
    ESP_LOGI(TAG, "Restarting");
    esp_restart();
}

static void register_restart(void)
{
    const esp_console_cmd_t cmd = {
        .command = "restart",
        .help = "Software reset of the chip",
        .hint = NULL,
        .func = &restart,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

/** 'free' command prints available heap memory */

static int free_mem(int argc, char **argv)
{
    printf("%d\n", esp_get_free_heap_size());
    return 0;
}

static void register_free(void)
{
    const esp_console_cmd_t cmd = {
        .command = "free",
        .help = "Get the current size of free heap memory",
        .hint = NULL,
        .func = &free_mem,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

/* 'heap' command prints minumum heap size */
static int heap_size(int argc, char **argv)
{
    uint32_t heap_size = heap_caps_get_minimum_free_size(MALLOC_CAP_DEFAULT);
    printf("min heap size: %u\n", heap_size);
    return 0;
}

static void register_heap(void)
{
    const esp_console_cmd_t heap_cmd = {
        .command = "heap",
        .help = "Get minimum size of free heap memory that was available during program execution",
        .hint = NULL,
        .func = &heap_size,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&heap_cmd) );

}

/** 'tasks' command prints the list of tasks and related information */
#if WITH_TASKS_INFO

static int tasks_info(int argc, char **argv)
{
    const size_t bytes_per_task = 40; /* see vTaskList description */
    char *task_list_buffer = malloc(uxTaskGetNumberOfTasks() * bytes_per_task);
    if (task_list_buffer == NULL) {
        ESP_LOGE(TAG, "failed to allocate buffer for vTaskList output");
        return 1;
    }
    fputs("Task Name\tStatus\tPrio\tHWM\tTask#", stdout);
#ifdef CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID
    fputs("\tAffinity", stdout);
#endif
    fputs("\n", stdout);
    vTaskList(task_list_buffer);
    fputs(task_list_buffer, stdout);
    free(task_list_buffer);
    return 0;
}

static void register_tasks(void)
{
    const esp_console_cmd_t cmd = {
        .command = "tasks",
        .help = "Get information about running tasks",
        .hint = NULL,
        .func = &tasks_info,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

#endif // WITH_TASKS_INFO

/** 'deep_sleep' command puts the chip into deep sleep mode */

static struct {
    struct arg_int *wakeup_time;
#if SOC_PM_SUPPORT_EXT_WAKEUP
    struct arg_int *wakeup_gpio_num;
    struct arg_int *wakeup_gpio_level;
#endif
    struct arg_end *end;
} deep_sleep_args;


static int deep_sleep(int argc, char **argv)
{
    int nerrors = arg_parse(argc, argv, (void **) &deep_sleep_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, deep_sleep_args.end, argv[0]);
        return 1;
    }
    if (deep_sleep_args.wakeup_time->count) {
        uint64_t timeout = 1000ULL * deep_sleep_args.wakeup_time->ival[0];
        ESP_LOGI(TAG, "Enabling timer wakeup, timeout=%lluus", timeout);
        ESP_ERROR_CHECK( esp_sleep_enable_timer_wakeup(timeout) );
    }

#if SOC_PM_SUPPORT_EXT_WAKEUP
    if (deep_sleep_args.wakeup_gpio_num->count) {
        int io_num = deep_sleep_args.wakeup_gpio_num->ival[0];
        if (!esp_sleep_is_valid_wakeup_gpio(io_num)) {
            ESP_LOGE(TAG, "GPIO %d is not an RTC IO", io_num);
            return 1;
        }
        int level = 0;
        if (deep_sleep_args.wakeup_gpio_level->count) {
            level = deep_sleep_args.wakeup_gpio_level->ival[0];
            if (level != 0 && level != 1) {
                ESP_LOGE(TAG, "Invalid wakeup level: %d", level);
                return 1;
            }
        }
        ESP_LOGI(TAG, "Enabling wakeup on GPIO%d, wakeup on %s level",
                 io_num, level ? "HIGH" : "LOW");

        ESP_ERROR_CHECK( esp_sleep_enable_ext1_wakeup(1ULL << io_num, level) );
        ESP_LOGE(TAG, "GPIO wakeup from deep sleep currently unsupported on ESP32-C3");
    }
#endif // SOC_PM_SUPPORT_EXT_WAKEUP

#if CONFIG_IDF_TARGET_ESP32
    rtc_gpio_isolate(GPIO_NUM_12);
#endif //CONFIG_IDF_TARGET_ESP32

    esp_deep_sleep_start();
}

static void register_deep_sleep(void)
{
    int num_args = 1;
    deep_sleep_args.wakeup_time =
        arg_int0(NULL, NULL, "<t>", "Wake up time, ms");
#if SOC_PM_SUPPORT_EXT_WAKEUP
    deep_sleep_args.wakeup_gpio_num =
        arg_int0(NULL, NULL, "<n>",
                 "If specified, wakeup using GPIO with given number");
    deep_sleep_args.wakeup_gpio_level =
        arg_int0(NULL, NULL, "<0|1>", "GPIO level to trigger wakeup");
    num_args += 2;
#endif
    deep_sleep_args.end = arg_end(num_args);

    const esp_console_cmd_t cmd = {
        .command = "deep_sleep",
        .help = "Enter deep sleep mode. "
#if SOC_PM_SUPPORT_EXT_WAKEUP
        "Two wakeup modes are supported: timer and GPIO. "
#else
        "Timer wakeup mode is supported. "
#endif
        "If no wakeup option is specified, will sleep indefinitely.",
        .hint = NULL,
        .func = &deep_sleep,
        .argtable = &deep_sleep_args
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

/** 'light_sleep' command puts the chip into light sleep mode */

static struct {
    struct arg_int *wakeup_time;
    struct arg_int *wakeup_gpio_num;
    struct arg_int *wakeup_gpio_level;
    struct arg_end *end;
} light_sleep_args;

static int light_sleep(int argc, char **argv)
{
    int nerrors = arg_parse(argc, argv, (void **) &light_sleep_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, light_sleep_args.end, argv[0]);
        return 1;
    }
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
    if (light_sleep_args.wakeup_time->count) {
        uint64_t timeout = 1000ULL * light_sleep_args.wakeup_time->ival[0];
        ESP_LOGI(TAG, "Enabling timer wakeup, timeout=%lluus", timeout);
        ESP_ERROR_CHECK( esp_sleep_enable_timer_wakeup(timeout) );
    }
    int io_count = light_sleep_args.wakeup_gpio_num->count;
    if (io_count != light_sleep_args.wakeup_gpio_level->count) {
        ESP_LOGE(TAG, "Should have same number of 'io' and 'io_level' arguments");
        return 1;
    }
    for (int i = 0; i < io_count; ++i) {
        int io_num = light_sleep_args.wakeup_gpio_num->ival[i];
        int level = light_sleep_args.wakeup_gpio_level->ival[i];
        if (level != 0 && level != 1) {
            ESP_LOGE(TAG, "Invalid wakeup level: %d", level);
            return 1;
        }
        ESP_LOGI(TAG, "Enabling wakeup on GPIO%d, wakeup on %s level",
                 io_num, level ? "HIGH" : "LOW");

        ESP_ERROR_CHECK( gpio_wakeup_enable(io_num, level ? GPIO_INTR_HIGH_LEVEL : GPIO_INTR_LOW_LEVEL) );
    }
    if (io_count > 0) {
        ESP_ERROR_CHECK( esp_sleep_enable_gpio_wakeup() );
    }
    if (CONFIG_ESP_CONSOLE_UART_NUM >= 0 && CONFIG_ESP_CONSOLE_UART_NUM <= UART_NUM_1) {
        ESP_LOGI(TAG, "Enabling UART wakeup (press ENTER to exit light sleep)");
        ESP_ERROR_CHECK( uart_set_wakeup_threshold(CONFIG_ESP_CONSOLE_UART_NUM, 3) );
        ESP_ERROR_CHECK( esp_sleep_enable_uart_wakeup(CONFIG_ESP_CONSOLE_UART_NUM) );
    }
    fflush(stdout);
    fsync(fileno(stdout));
    esp_light_sleep_start();
    esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
    const char *cause_str;
    switch (cause) {
    case ESP_SLEEP_WAKEUP_GPIO:
        cause_str = "GPIO";
        break;
    case ESP_SLEEP_WAKEUP_UART:
        cause_str = "UART";
        break;
    case ESP_SLEEP_WAKEUP_TIMER:
        cause_str = "timer";
        break;
    default:
        cause_str = "unknown";
        printf("%d\n", cause);
    }
    ESP_LOGI(TAG, "Woke up from: %s", cause_str);
    return 0;
}

static void register_light_sleep(void)
{
    light_sleep_args.wakeup_time =
        arg_int0(NULL, NULL, "<t>", "Wake up time, ms");
    light_sleep_args.wakeup_gpio_num =
        arg_intn(NULL, NULL, "<n>", 0, 8,
                 "If specified, wakeup using GPIO with given number");
    light_sleep_args.wakeup_gpio_level =
        arg_intn(NULL, "io_level", "<0|1>", 0, 8, "GPIO level to trigger wakeup");
    light_sleep_args.end = arg_end(3);

    const esp_console_cmd_t cmd = {
        .command = "light_sleep",
        .help = "Enter light sleep mode. "
        "Two wakeup modes are supported: timer and GPIO. "
        "Multiple GPIO pins can be specified using pairs of "
        "'io' and 'io_level' arguments. "
        "Will also wake up on UART input.",
        .hint = NULL,
        .func = &light_sleep,
        .argtable = &light_sleep_args
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}