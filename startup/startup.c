extern int main(void);

extern char _estack;
extern char _sidata;
extern char _sdata;
extern char _edata;
extern char _sbss;
extern char _ebss;

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
  char *start_data_src =  &_sidata;
  char *start_data_dest = &_sdata;
  char *end_data_dest = &_edata;

  while (start_data_dest != end_data_dest) {
    *start_data_dest++ = *start_data_src++;
  }

  char *start_bss_dest = &_sbss;
  char *end_bss_dest = &_ebss;

  while (start_bss_dest != end_bss_dest) {
    *start_bss_dest++ = 0;
  }

  main();
}

void HardFault_Handler(void) {
  while(1);
}
