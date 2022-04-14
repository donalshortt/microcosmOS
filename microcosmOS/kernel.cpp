#define VIDEO_START 0xb8000
#define VGA_LIGHT_GRAY 7
#define VGA_RED 4

static void PrintString(const char *str)
{
  unsigned char *video = ((unsigned char *)VIDEO_START);
  while(*str != '\0') {
    *(video++) = *str++;
    *(video++) = VGA_RED;
  }
}

extern "C" void
kernel_main(void)
{
  PrintString("It is I, Albert Pennysworth");

  PrintString("Great heavens!");

  while(1);
}
