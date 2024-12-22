#include "idt.h"
#include "pic.h"
#include "../../../drivers/vga/vga.h"

idt::idt_entry idt_entries[idt::numOfEntries] = {0};
idt::idt_ptr_struct idtr;

extern "C" void idtFlush(uint32_t);

void idt::init()
{
    idtr.base = (uint32_t)idt_entries;
    idtr.limit = (sizeof(idt_entry) * idt::numOfEntries) - 1;

    pic::init();
    initISA();
    initIRQ();

    idtFlush((uint32_t)&idtr);

    vga::kprintf("Finished Initializing IDT\n");
}

void idt::insertEntry(uint32_t index, uint32_t offset, uint16_t selector, uint8_t type_attributes)
{
    idt_entries[index].selector = selector;
    idt_entries[index].type_attributes = type_attributes;
    idt_entries[index].offset_low = offset & 0xFFFF;
    idt_entries[index].zero = 0;
    idt_entries[index].offset_high = offset >> 16;
}

char *exception_descriptions[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    "Reserved"};

void pageFaultHandler()
{
    uint32_t faultingAddress;
    asm volatile("mov %%cr2, %0" : "=r"(faultingAddress)); // Get the address that caused the fault
    vga::kprintf("Page fault at address: %x\n", faultingAddress);
}
extern "C" void idt::isrHandler(stack_frame *frame)
{
    if (frame->int_num == 14)
    {
        pageFaultHandler();
    }
    if (frame->int_num < 32)
    {
        vga::puts(exception_descriptions[frame->int_num]);
        vga::puts("\nHalted CPU(infinity loop)");
        for (;;)
            ;
    }
}

idt::Handler irqHandlers[16] = {0};

extern "C" void idt::irqHandler(stack_frame *frame)
{
    Handler handler = irqHandlers[frame->int_num - 32];

    if (handler)
    {
        handler(frame);
    }

    pic::sendEOI(frame->int_num);
}

void idt::irqInstallHandler(int irqNumber, void (*handler)(stack_frame *frame))
{
    irqHandlers[irqNumber] = handler;
}

void idt::irqUninstallHandler(int irqNumber)
{
    irqHandlers[irqNumber] = 0;
}

void idt::initISA()
{
    insertEntry(0, (uint32_t)isr0, 0x08, 0x8E);
    insertEntry(1, (uint32_t)isr1, 0x08, 0x8E);
    insertEntry(2, (uint32_t)isr2, 0x08, 0x8E);
    insertEntry(3, (uint32_t)isr3, 0x08, 0x8E);
    insertEntry(4, (uint32_t)isr4, 0x08, 0x8E);
    insertEntry(5, (uint32_t)isr5, 0x08, 0x8E);
    insertEntry(6, (uint32_t)isr6, 0x08, 0x8E);
    insertEntry(7, (uint32_t)isr7, 0x08, 0x8E);
    insertEntry(8, (uint32_t)isr8, 0x08, 0x8E);
    insertEntry(9, (uint32_t)isr9, 0x08, 0x8E);
    insertEntry(10, (uint32_t)isr10, 0x08, 0x8E);
    insertEntry(11, (uint32_t)isr11, 0x08, 0x8E);
    insertEntry(12, (uint32_t)isr12, 0x08, 0x8E);
    insertEntry(13, (uint32_t)isr13, 0x08, 0x8E);
    insertEntry(14, (uint32_t)isr14, 0x08, 0x8E);
    insertEntry(15, (uint32_t)isr15, 0x08, 0x8E);
    insertEntry(16, (uint32_t)isr16, 0x08, 0x8E);
    insertEntry(17, (uint32_t)isr17, 0x08, 0x8E);
    insertEntry(18, (uint32_t)isr18, 0x08, 0x8E);
    insertEntry(19, (uint32_t)isr19, 0x08, 0x8E);
    insertEntry(20, (uint32_t)isr20, 0x08, 0x8E);
    insertEntry(21, (uint32_t)isr21, 0x08, 0x8E);
    insertEntry(22, (uint32_t)isr22, 0x08, 0x8E);
    insertEntry(23, (uint32_t)isr23, 0x08, 0x8E);
    insertEntry(24, (uint32_t)isr24, 0x08, 0x8E);
    insertEntry(25, (uint32_t)isr25, 0x08, 0x8E);
    insertEntry(26, (uint32_t)isr26, 0x08, 0x8E);
    insertEntry(27, (uint32_t)isr27, 0x08, 0x8E);
    insertEntry(28, (uint32_t)isr28, 0x08, 0x8E);
    insertEntry(29, (uint32_t)isr29, 0x08, 0x8E);
    insertEntry(30, (uint32_t)isr30, 0x08, 0x8E);
    insertEntry(31, (uint32_t)isr31, 0x08, 0x8E);
}

void idt::initIRQ()
{
    insertEntry(32, (uint32_t)irq0, 0x08, 0x8E);
    insertEntry(33, (uint32_t)irq1, 0x08, 0x8E);
    insertEntry(34, (uint32_t)irq2, 0x08, 0x8E);
    insertEntry(35, (uint32_t)irq3, 0x08, 0x8E);
    insertEntry(36, (uint32_t)irq4, 0x08, 0x8E);
    insertEntry(37, (uint32_t)irq5, 0x08, 0x8E);
    insertEntry(38, (uint32_t)irq6, 0x08, 0x8E);
    insertEntry(39, (uint32_t)irq7, 0x08, 0x8E);
    insertEntry(40, (uint32_t)irq8, 0x08, 0x8E);
    insertEntry(41, (uint32_t)irq9, 0x08, 0x8E);
    insertEntry(42, (uint32_t)irq10, 0x08, 0x8E);
    insertEntry(43, (uint32_t)irq11, 0x08, 0x8E);
    insertEntry(44, (uint32_t)irq12, 0x08, 0x8E);
    insertEntry(45, (uint32_t)irq13, 0x08, 0x8E);
    insertEntry(46, (uint32_t)irq14, 0x08, 0x8E);
    insertEntry(47, (uint32_t)irq15, 0x08, 0x8E);
}