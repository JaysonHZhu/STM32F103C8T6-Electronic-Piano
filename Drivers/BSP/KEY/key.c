#include "key.h"
#include "./SYSTEM/delay/delay.h"

#define KEY_EXIT_WINDOW_MS   100U   /* 两键需在此时长内先后按下才算退出组合键 */

static uint8_t  exit_armed = 0;         /* 已按下其中一个键，等待另一个 */
static uint32_t exit_first_tick = 0;    /* 第一个键按下的时刻 */
static uint16_t seq_counter = 0;        /* 按键顺序号，用于“最近按下的键优先” */

void KEY_Init(void)
{
    GPIO_InitTypeDef gpio = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();
    gpio.Pin = KEY_UP_PIN | KEY_DOWN_PIN | KEY_CONFIRM_PIN |
               KEY1_PIN | KEY2_PIN | KEY3_PIN | KEY4_PIN;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &gpio);
}

KeyValue KEY_Scan(void)
{
    static uint8_t last[7] = {1,1,1,1,1,1,1};
    static uint8_t hold[7] = {0,0,0,0,0,0,0};
    static uint16_t press_seq[7] = {0,0,0,0,0,0,0};   /* 每个键最近一次按下的顺序号 */
    const uint16_t pins[7] = {KEY_UP_PIN, KEY_DOWN_PIN, KEY_CONFIRM_PIN,
                              KEY1_PIN, KEY2_PIN, KEY3_PIN, KEY4_PIN};
    const KeyValue map[7] = {KEY_UP, KEY_DOWN, KEY_CONFIRM,
                             KEY_1, KEY_2, KEY_3, KEY_4};

    uint32_t now = HAL_GetTick();
    uint8_t e1 = HAL_GPIO_ReadPin(KEY_PORT, KEY_EXIT1_PIN);
    uint8_t e2 = HAL_GPIO_ReadPin(KEY_PORT, KEY_EXIT2_PIN);

    /* 退出组合键要求两键“几乎同时”按下：
       若先按住 key6 再按 key7（或反过来），两键间隔超过 KEY_EXIT_WINDOW_MS，
       就按普通按键处理，钢琴模式下才能连着弹 La -> Si。 */
    if (e1 == 0 || e2 == 0) {
        if (!exit_armed) {
            exit_armed = 1;
            exit_first_tick = now;
        }
        if (e1 == 0 && e2 == 0 && (now - exit_first_tick) <= KEY_EXIT_WINDOW_MS) {
            return KEY_EXIT;
        }
    } else {
        exit_armed = 0;                 /* 两键都松开，重新武装 */
    }

    uint8_t best = 0xFF;
    for (uint8_t i = 0; i < 7; i++) {
        uint8_t cur = HAL_GPIO_ReadPin(KEY_PORT, pins[i]);
        if (last[i] == 1 && cur == 0) {
            hold[i] = 1;
            press_seq[i] = ++seq_counter;      /* 后按下的键优先，按住 key6 再按 key7 会切到 Si */
        } else if (cur == 1) {
            hold[i] = 0;
        }
        last[i] = cur;
        if (hold[i] && (best == 0xFF || press_seq[i] > press_seq[best])) {
            best = i;
        }
    }
    if (best != 0xFF) return map[best];

    seq_counter = 0;    /* 全部松开后复位顺序号，避免累加溢出 */
    return KEY_NONE;
}

KeyValue KEY_Get(void)
{
    KeyValue k;
    while (1) {
        sys_iwdg_feed();                /* 菜单里是阻塞等待，必须持续喂狗 */
        k = KEY_Scan();
        if (k != KEY_NONE) {
            delay_ms(20);
            while (KEY_Scan() != KEY_NONE) {
                sys_iwdg_feed();        /* 等待松键期间同样喂狗 */
            }
            return k;
        }
    }
}

uint8_t KEY_IsExitPressed(void)
{
    return (KEY_Scan() == KEY_EXIT);
}
