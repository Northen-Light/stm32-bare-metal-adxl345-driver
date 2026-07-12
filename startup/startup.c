extern int main(void);

extern char _estack;
extern char _end_rodata_;
extern char _start_data_;
extern char _end_data_;
extern char _start_bss_;
extern char _end_bss_;

void Reset_Handler(void);
void HardFault_Handler(void);

__attribute__((section(".isr_vector")))
char *vector_table[] = {
  &_estack,
  (char *)Reset_Handler,
  0,
  (char *)HardFault_Handler
};

void Reset_Handler(void) {
  char *start_data_src = &_end_rodata_;
  char *start_data_dest = &_start_data_;
  char *end_data_dest = &_end_data_;

  while (start_data_dest != end_data_dest) {
    *start_data_dest++ = *start_data_src++;
  }

  char *start_bss_dest = &_start_bss_;
  char *end_bss_dest = &_end_bss_;

  while (start_bss_dest != end_bss_dest) {
    *start_bss_dest++ = 0;
  }

  main();
}

void HardFault_Handler(void) {
  while(1);
}
