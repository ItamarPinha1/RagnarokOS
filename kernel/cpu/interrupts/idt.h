#pragma once
#include <stdint.h>

namespace idt
{
    struct idt_entry
    {
        uint16_t offset_low;
        uint16_t selector;
        uint8_t zero;
        uint8_t type_attributes;
        uint16_t offset_high;
    } __attribute__((packed));

    struct idt_ptr_struct
    {
        uint16_t limit;
        uint32_t base;
    } __attribute__((packed));

    struct stack_frame
    {
        uint32_t cr2;
        uint32_t ds;
        uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
        uint32_t int_num, err_code;
        uint32_t eip, csm, eflags, useresp, ss;
    } __attribute__((packed));

    void init();
    void initISA();
    void initIRQ();
    void insertEntry(uint32_t index, uint32_t offset, uint16_t selector, uint8_t type_attributes);

    extern "C" void isrHandler(stack_frame *frame);
    extern "C" void irqHandler(stack_frame *frame);

    void irqInstallHandler(int irq, void (*handler)(stack_frame *frame));
    void irqUninstallHandler(int irqNumber);

    const int numOfEntries = 256;

    typedef void (*Handler)(stack_frame *frame);
};

extern "C" void isr0();
extern "C" void isr1();
extern "C" void isr2();
extern "C" void isr3();
extern "C" void isr4();
extern "C" void isr5();
extern "C" void isr6();
extern "C" void isr7();
extern "C" void isr8();
extern "C" void isr9();
extern "C" void isr10();
extern "C" void isr11();
extern "C" void isr12();
extern "C" void isr13();
extern "C" void isr14();
extern "C" void isr15();
extern "C" void isr16();
extern "C" void isr17();
extern "C" void isr18();
extern "C" void isr19();
extern "C" void isr20();
extern "C" void isr21();
extern "C" void isr22();
extern "C" void isr23();
extern "C" void isr24();
extern "C" void isr25();
extern "C" void isr26();
extern "C" void isr27();
extern "C" void isr28();
extern "C" void isr29();
extern "C" void isr30();
extern "C" void isr31();

extern "C" void irq0();
extern "C" void irq1();
extern "C" void irq2();
extern "C" void irq3();
extern "C" void irq4();
extern "C" void irq5();
extern "C" void irq6();
extern "C" void irq7();
extern "C" void irq8();
extern "C" void irq9();
extern "C" void irq10();
extern "C" void irq11();
extern "C" void irq12();
extern "C" void irq13();
extern "C" void irq14();
extern "C" void irq15();