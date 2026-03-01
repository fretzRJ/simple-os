ENTRY(_start)

PHDRS
{
  text PT_LOAD FLAGS(5);    /* R + X */
  rodata PT_LOAD FLAGS(4);  /* R */
  data PT_LOAD FLAGS(6);    /* R + W */
}

SECTIONS
{
  . = 1M;

  .multiboot :
  {
    *(.multiboot)
  } :text

  .text :
  {
    *(.text*)
  } :text

  .rodata :
  {
    *(.rodata*)
  } :rodata

  .data :
  {
    *(.data*)
  } :data

  .bss :
  {
    *(COMMON)
    *(.bss*)
  } :data
}
