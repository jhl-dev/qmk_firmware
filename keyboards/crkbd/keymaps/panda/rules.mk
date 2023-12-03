MOUSEKEY_ENABLE = yes    # Mouse keys
OLED_ENABLE     = yes
OLED_DRIVER     = SSD1306
LTO_ENABLE      = yes
SRC += oneshot.c
CFLAGS += -fcommon
KEY_OVERRIDE_ENABLE = yes