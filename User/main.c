#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/OLED/oled.h"
#include "./BSP/BUZZER/buzzer.h"
#include "./BSP/KEY/key.h"

typedef enum { MODE_MENU, MODE_PIANO, MODE_AUTOPLAY, MODE_MARQUEE } SystemMode;
typedef enum { MENU_PIANO = 0, MENU_AUTOPLAY, MENU_MARQUEE, MENU_COUNT } MenuItem;
typedef enum { AUTO_SELECT, AUTO_PLAYING } AutoSub;

static SystemMode currentMode = MODE_MENU;
static MenuItem currentSelection = MENU_PIANO;
static AutoSub autoSub = AUTO_SELECT;
static uint8_t songSel = 0;

static const uint16_t noteFreq[] = {262, 294, 330, 349, 392, 440, 494, 523, 587, 659, 698, 784, 880, 988};

static void Menu_Display(void)
{
    const char *items[] = {"1.Piano", "2.AutoPlay", "3.Marquee"};
    OLED_RefreshControl(0);
    OLED_Clear();
    for (uint8_t i = 0; i < MENU_COUNT; i++) {
        OLED_ShowChar(0, 1 + i * 2, (i == currentSelection) ? '>' : ' ');
        OLED_ShowString_Fast(8, 1 + i * 2, (uint8_t *)items[i]);
    }
    OLED_ShowString_Fast(0, 7, (uint8_t *)"key1:U key2:D key3:OK");
    OLED_RefreshControl(1);
}

static void EnterPiano(void)
{
    currentMode = MODE_PIANO;
    OLED_RefreshControl(0);
    OLED_Clear();
    OLED_ShowString_Fast(0, 0, (uint8_t *)"*** Piano ***");
    OLED_ShowString_Fast(0, 2, (uint8_t *)"key1:Do key2:Re");
    OLED_ShowString_Fast(0, 3, (uint8_t *)"key3:Mi key4:Fa");
    OLED_ShowString_Fast(0, 4, (uint8_t *)"key5:So key6:La");
    OLED_ShowString_Fast(0, 5, (uint8_t *)"key7:Si");
    OLED_ShowString_Fast(0, 7, (uint8_t *)"key6+key7 = Exit");
    OLED_RefreshControl(1);
}

static void RunPiano(void)
{
    static KeyValue lastKey = KEY_NONE;
    if (KEY_IsExitPressed()) { BUZZER_Off(); LED_AllOff(); lastKey = KEY_NONE; delay_ms(200); currentMode = MODE_MENU; return; }
    KeyValue k = KEY_Scan();
    if (k == lastKey) return;
    lastKey = k;
    uint16_t f = 0;
    uint8_t led = 0;
    if (k == KEY_UP) { f = noteFreq[0]; led = 0; }
    else if (k == KEY_DOWN) { f = noteFreq[1]; led = 1; }
    else if (k == KEY_CONFIRM) { f = noteFreq[2]; led = 2; }
    else if (k == KEY_1) { f = noteFreq[3]; led = 3; }
    else if (k == KEY_2) { f = noteFreq[4]; led = 4; }
    else if (k == KEY_3) { f = noteFreq[5]; led = 5; }
    else if (k == KEY_4) { f = noteFreq[6]; led = 6; }
    if (f) { BUZZER_SetFreq(f); LED_Set(1 << led); }
    else { BUZZER_Off(); LED_AllOff(); }
}

static const uint8_t song1[] = {
    1,6,4, 1,38,4, 1,74,4, 1,6,4,
    1,6,4, 1,38,4, 1,74,4, 1,6,4,
    1,74,4, 1,93,4, 2,8,4, 1,74,4,
    1,93,4, 2,8,8, 2,8,4,
    2,72,4, 2,8,4, 1,93,4, 1,74,4,
    1,6,4, 2,8,4, 2,72,4, 2,8,4,
    1,93,4, 1,74,4, 1,6,4,
    1,38,4, 1,6,8, 0xFF
};
static const uint8_t song2[] = {
    1,6,4, 1,6,4, 2,8,4, 2,8,4,
    2,72,4, 2,72,4, 2,8,8,
    2,72,4, 2,8,4, 1,93,4, 1,93,4,
    1,74,4, 1,74,4, 1,38,8,
    2,8,4, 2,8,4, 1,93,4, 1,93,4,
    1,74,4, 1,74,4, 1,38,8,
    2,8,4, 2,8,4, 2,72,4, 2,72,4,
    2,8,8, 1,6,4, 1,6,4, 2,8,4, 2,8,4,
    2,72,4, 2,72,4, 2,8,8,
    2,72,4, 2,8,4, 1,93,4, 1,93,4,
    1,74,4, 1,74,4, 1,38,8,
    1,6,4, 1,6,4, 2,8,4, 2,8,4,
    2,72,4, 2,72,4, 2,8,8, 0xFF
};
static const uint8_t song3[] = {
    1,74,4, 1,74,4, 1,93,4, 2,8,4,
    2,8,4, 1,93,4, 1,74,4, 1,38,4,
    1,6,4, 1,6,4, 1,38,4, 1,74,4,
    1,74,8, 1,38,4, 1,38,4,
    1,74,4, 1,93,4, 2,8,4, 2,8,4,
    1,93,4, 1,74,4, 1,38,4, 1,6,4,
    1,38,4, 1,74,4, 1,74,8,
    1,38,4, 1,38,4, 1,74,4, 1,93,4,
    2,8,4, 2,8,4, 1,93,4, 1,74,4,
    1,38,4, 1,6,4, 1,38,4, 1,74,4,
    1,38,8, 1,38,8, 0xFF
};

typedef struct { const uint8_t *data; const char *name; } Song;
static const Song songs[] = {
    {song1, "Two Tigers"},
    {song2, "Twinkle Star"},
    {song3, "Ode To Joy"},
};
#define SONG_NUM (sizeof(songs) / sizeof(songs[0]))
static uint16_t autoPos = 0;

static void EnterAutoSelect(void)
{
    autoSub = AUTO_SELECT;
    OLED_RefreshControl(0);
    OLED_Clear();
    OLED_ShowString_Fast(0, 0, (uint8_t *)"Select Song:");
    for (uint8_t i = 0; i < SONG_NUM; i++) {
        OLED_ShowChar(2, 2 + i * 2, (i == songSel) ? '>' : ' ');
        OLED_ShowString_Fast(10, 2 + i * 2, (uint8_t *)songs[i].name);
    }
    OLED_RefreshControl(1);
}

static void EnterAutoPlay(void)
{
    currentMode = MODE_AUTOPLAY;
    EnterAutoSelect();
}

static void RunAutoPlay(void)
{
    if (autoSub == AUTO_SELECT) {
        static KeyValue lastKey = KEY_NONE;
        KeyValue k = KEY_Scan();
        if (k == KEY_EXIT) { lastKey = KEY_NONE; delay_ms(200); currentMode = MODE_MENU; return; }
        if (k != KEY_NONE && k != lastKey) {
            if (k == KEY_UP && songSel > 0) { songSel--; EnterAutoSelect(); }
            else if (k == KEY_DOWN && songSel < SONG_NUM - 1) { songSel++; EnterAutoSelect(); }
            else if (k == KEY_CONFIRM) {
                autoPos = 0; autoSub = AUTO_PLAYING;
                OLED_RefreshControl(0);
                OLED_Clear();
                OLED_ShowString_Fast(0, 0, (uint8_t *)"*** Playing ***");
                OLED_ShowString_Fast(0, 2, (uint8_t *)songs[songSel].name);
                OLED_ShowString_Fast(0, 7, (uint8_t *)"key6+key7 = Exit");
                OLED_RefreshControl(1);
            }
        }
        if (k == KEY_NONE) lastKey = KEY_NONE; else lastKey = k;
        delay_ms(50);
        return;
    }

    if (KEY_IsExitPressed()) { BUZZER_Off(); LED_AllOff(); HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); delay_ms(200); autoSub = AUTO_SELECT; currentMode = MODE_MENU; return; }
    const uint8_t *data = songs[songSel].data;
    if (data[autoPos] == 0xFF) { BUZZER_Off(); LED_AllOff(); HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); delay_ms(500); autoSub = AUTO_SELECT; EnterAutoSelect(); return; }
    uint16_t f = ((uint16_t)data[autoPos] << 8) | data[autoPos + 1];
    uint8_t d = data[autoPos + 2];
    autoPos += 3;
    if (f) {
        BUZZER_SetFreq(f);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    } else {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
    }
    delay_ms(d * 100);
    BUZZER_Off();
    delay_ms(20);
}

static void EnterMarquee(void)
{
    currentMode = MODE_MARQUEE;
    LED_AllOff();
    OLED_RefreshControl(0);
    OLED_Clear();
    OLED_ShowString_Fast(0, 0, (uint8_t *)"*** Marquee ***");
    OLED_ShowString_Fast(0, 2, (uint8_t *)"LED Chaser");
    OLED_ShowString_Fast(0, 7, (uint8_t *)"key6+key7 = Exit");
    OLED_RefreshControl(1);
}

static uint8_t marqueeDir = 0, marqueePos = 0;

static void RunMarquee(void)
{
    if (KEY_IsExitPressed()) { LED_AllOff(); delay_ms(200); currentMode = MODE_MENU; return; }
    LED_Set(1 << marqueePos);
    delay_ms(120);
    if (!marqueeDir) { marqueePos++; if (marqueePos >= 7) { marqueePos = 6; marqueeDir = 1; } }
    else { if (marqueePos == 0) marqueeDir = 0; else marqueePos--; }
}

int main(void)
{
    HAL_Init();
    sys_stm32_clock_init(RCC_PLL_MUL9);
    delay_init(72);
    usart_init(115200); /* 串口调试：printf 输出到 PA9/PA10 */
    led_Init();
    OLED_Init();
    BUZZER_Init();
    KEY_Init();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = GPIO_PIN_13;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &gpio);

    printf("STM32 Electronic Piano started.\r\n");

    while (1) {
        switch (currentMode) {
            case MODE_MENU: {
                Menu_Display();
                switch (KEY_Get()) {
                    case KEY_UP:    if (currentSelection > 0) currentSelection--; break;
                    case KEY_DOWN:  if (currentSelection < MENU_COUNT - 1) currentSelection++; break;
                    case KEY_CONFIRM:
                        if (currentSelection == MENU_PIANO)    EnterPiano();
                        else if (currentSelection == MENU_AUTOPLAY) EnterAutoPlay();
                        else if (currentSelection == MENU_MARQUEE) EnterMarquee();
                        break;
                    default: break;
                }
                break;
            }
            case MODE_PIANO:    RunPiano();    break;
            case MODE_AUTOPLAY: RunAutoPlay(); break;
            case MODE_MARQUEE:  RunMarquee();  break;
        }
    }
}
