void idle();

int kernel_entry (void) {
    idle();
    return 0;
}