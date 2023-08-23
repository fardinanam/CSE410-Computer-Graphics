static unsigned long int g_seed = 1;
inline int my_rand() {
    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
}