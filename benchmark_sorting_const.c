#include <arm_neon.h>
#include <stdio.h>

#define BUF_LEN 528

const uint8_t table_idx[256][16] = {
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 0
    {0, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 1
    {2, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 2
    {0, 1, 2, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 3
    {4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 4
    {0, 1, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 5
    {2, 3, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 6
    {0, 1, 2, 3, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},       // 7
    {6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 8
    {0, 1, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 9
    {2, 3, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 10
    {0, 1, 2, 3, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},       // 11
    {4, 5, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 12
    {0, 1, 4, 5, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},       // 13
    {2, 3, 4, 5, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},       // 14
    {0, 1, 2, 3, 4, 5, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1},         // 15
    {8, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 16
    {0, 1, 8, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 17
    {2, 3, 8, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 18
    {0, 1, 2, 3, 8, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},       // 19
    {4, 5, 8, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 20
    {0, 1, 4, 5, 8, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},       // 21
    {2, 3, 4, 5, 8, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},       // 22
    {0, 1, 2, 3, 4, 5, 8, 9, -1, -1, -1, -1, -1, -1, -1, -1},         // 23
    {6, 7, 8, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 24
    {0, 1, 6, 7, 8, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},       // 25
    {2, 3, 6, 7, 8, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},       // 26
    {0, 1, 2, 3, 6, 7, 8, 9, -1, -1, -1, -1, -1, -1, -1, -1},         // 27
    {4, 5, 6, 7, 8, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},       // 28
    {0, 1, 4, 5, 6, 7, 8, 9, -1, -1, -1, -1, -1, -1, -1, -1},         // 29
    {2, 3, 4, 5, 6, 7, 8, 9, -1, -1, -1, -1, -1, -1, -1, -1},         // 30
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1, -1, -1, -1, -1, -1},           // 31
    {10, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 32
    {0, 1, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 33
    {2, 3, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 34
    {0, 1, 2, 3, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 35
    {4, 5, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 36
    {0, 1, 4, 5, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 37
    {2, 3, 4, 5, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 38
    {0, 1, 2, 3, 4, 5, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1},       // 39
    {6, 7, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 40
    {0, 1, 6, 7, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 41
    {2, 3, 6, 7, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 42
    {0, 1, 2, 3, 6, 7, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1},       // 43
    {4, 5, 6, 7, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 44
    {0, 1, 4, 5, 6, 7, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1},       // 45
    {2, 3, 4, 5, 6, 7, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1},       // 46
    {0, 1, 2, 3, 4, 5, 6, 7, 10, 11, -1, -1, -1, -1, -1, -1},         // 47
    {8, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 48
    {0, 1, 8, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 49
    {2, 3, 8, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 50
    {0, 1, 2, 3, 8, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1},       // 51
    {4, 5, 8, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 52
    {0, 1, 4, 5, 8, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1},       // 53
    {2, 3, 4, 5, 8, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1},       // 54
    {0, 1, 2, 3, 4, 5, 8, 9, 10, 11, -1, -1, -1, -1, -1, -1},         // 55
    {6, 7, 8, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 56
    {0, 1, 6, 7, 8, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1},       // 57
    {2, 3, 6, 7, 8, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1},       // 58
    {0, 1, 2, 3, 6, 7, 8, 9, 10, 11, -1, -1, -1, -1, -1, -1},         // 59
    {4, 5, 6, 7, 8, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1},       // 60
    {0, 1, 4, 5, 6, 7, 8, 9, 10, 11, -1, -1, -1, -1, -1, -1},         // 61
    {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, -1, -1, -1, -1, -1, -1},         // 62
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, -1, -1, -1, -1},           // 63
    {12, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 64
    {0, 1, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 65
    {2, 3, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 66
    {0, 1, 2, 3, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 67
    {4, 5, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 68
    {0, 1, 4, 5, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 69
    {2, 3, 4, 5, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 70
    {0, 1, 2, 3, 4, 5, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1},       // 71
    {6, 7, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 72
    {0, 1, 6, 7, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 73
    {2, 3, 6, 7, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 74
    {0, 1, 2, 3, 6, 7, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1},       // 75
    {4, 5, 6, 7, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 76
    {0, 1, 4, 5, 6, 7, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1},       // 77
    {2, 3, 4, 5, 6, 7, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1},       // 78
    {0, 1, 2, 3, 4, 5, 6, 7, 12, 13, -1, -1, -1, -1, -1, -1},         // 79
    {8, 9, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 80
    {0, 1, 8, 9, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 81
    {2, 3, 8, 9, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 82
    {0, 1, 2, 3, 8, 9, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1},       // 83
    {4, 5, 8, 9, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 84
    {0, 1, 4, 5, 8, 9, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1},       // 85
    {2, 3, 4, 5, 8, 9, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1},       // 86
    {0, 1, 2, 3, 4, 5, 8, 9, 12, 13, -1, -1, -1, -1, -1, -1},         // 87
    {6, 7, 8, 9, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 88
    {0, 1, 6, 7, 8, 9, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1},       // 89
    {2, 3, 6, 7, 8, 9, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1},       // 90
    {0, 1, 2, 3, 6, 7, 8, 9, 12, 13, -1, -1, -1, -1, -1, -1},         // 91
    {4, 5, 6, 7, 8, 9, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1},       // 92
    {0, 1, 4, 5, 6, 7, 8, 9, 12, 13, -1, -1, -1, -1, -1, -1},         // 93
    {2, 3, 4, 5, 6, 7, 8, 9, 12, 13, -1, -1, -1, -1, -1, -1},         // 94
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 12, 13, -1, -1, -1, -1},           // 95
    {10, 11, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 96
    {0, 1, 10, 11, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 97
    {2, 3, 10, 11, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 98
    {0, 1, 2, 3, 10, 11, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1},     // 99
    {4, 5, 10, 11, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 100
    {0, 1, 4, 5, 10, 11, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1},     // 101
    {2, 3, 4, 5, 10, 11, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1},     // 102
    {0, 1, 2, 3, 4, 5, 10, 11, 12, 13, -1, -1, -1, -1, -1, -1},       // 103
    {6, 7, 10, 11, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 104
    {0, 1, 6, 7, 10, 11, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1},     // 105
    {2, 3, 6, 7, 10, 11, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1},     // 106
    {0, 1, 2, 3, 6, 7, 10, 11, 12, 13, -1, -1, -1, -1, -1, -1},       // 107
    {4, 5, 6, 7, 10, 11, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1},     // 108
    {0, 1, 4, 5, 6, 7, 10, 11, 12, 13, -1, -1, -1, -1, -1, -1},       // 109
    {2, 3, 4, 5, 6, 7, 10, 11, 12, 13, -1, -1, -1, -1, -1, -1},       // 110
    {0, 1, 2, 3, 4, 5, 6, 7, 10, 11, 12, 13, -1, -1, -1, -1},         // 111
    {8, 9, 10, 11, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 112
    {0, 1, 8, 9, 10, 11, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1},     // 113
    {2, 3, 8, 9, 10, 11, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1},     // 114
    {0, 1, 2, 3, 8, 9, 10, 11, 12, 13, -1, -1, -1, -1, -1, -1},       // 115
    {4, 5, 8, 9, 10, 11, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1},     // 116
    {0, 1, 4, 5, 8, 9, 10, 11, 12, 13, -1, -1, -1, -1, -1, -1},       // 117
    {2, 3, 4, 5, 8, 9, 10, 11, 12, 13, -1, -1, -1, -1, -1, -1},       // 118
    {0, 1, 2, 3, 4, 5, 8, 9, 10, 11, 12, 13, -1, -1, -1, -1},         // 119
    {6, 7, 8, 9, 10, 11, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1},     // 120
    {0, 1, 6, 7, 8, 9, 10, 11, 12, 13, -1, -1, -1, -1, -1, -1},       // 121
    {2, 3, 6, 7, 8, 9, 10, 11, 12, 13, -1, -1, -1, -1, -1, -1},       // 122
    {0, 1, 2, 3, 6, 7, 8, 9, 10, 11, 12, 13, -1, -1, -1, -1},         // 123
    {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, -1, -1, -1, -1, -1, -1},       // 124
    {0, 1, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, -1, -1, -1, -1},         // 125
    {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, -1, -1, -1, -1},         // 126
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, -1, -1},           // 127
    {14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 128
    {0, 1, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 129
    {2, 3, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 130
    {0, 1, 2, 3, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 131
    {4, 5, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 132
    {0, 1, 4, 5, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 133
    {2, 3, 4, 5, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 134
    {0, 1, 2, 3, 4, 5, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},       // 135
    {6, 7, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 136
    {0, 1, 6, 7, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 137
    {2, 3, 6, 7, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 138
    {0, 1, 2, 3, 6, 7, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},       // 139
    {4, 5, 6, 7, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 140
    {0, 1, 4, 5, 6, 7, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},       // 141
    {2, 3, 4, 5, 6, 7, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},       // 142
    {0, 1, 2, 3, 4, 5, 6, 7, 14, 15, -1, -1, -1, -1, -1, -1},         // 143
    {8, 9, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 144
    {0, 1, 8, 9, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 145
    {2, 3, 8, 9, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 146
    {0, 1, 2, 3, 8, 9, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},       // 147
    {4, 5, 8, 9, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 148
    {0, 1, 4, 5, 8, 9, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},       // 149
    {2, 3, 4, 5, 8, 9, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},       // 150
    {0, 1, 2, 3, 4, 5, 8, 9, 14, 15, -1, -1, -1, -1, -1, -1},         // 151
    {6, 7, 8, 9, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},     // 152
    {0, 1, 6, 7, 8, 9, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},       // 153
    {2, 3, 6, 7, 8, 9, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},       // 154
    {0, 1, 2, 3, 6, 7, 8, 9, 14, 15, -1, -1, -1, -1, -1, -1},         // 155
    {4, 5, 6, 7, 8, 9, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},       // 156
    {0, 1, 4, 5, 6, 7, 8, 9, 14, 15, -1, -1, -1, -1, -1, -1},         // 157
    {2, 3, 4, 5, 6, 7, 8, 9, 14, 15, -1, -1, -1, -1, -1, -1},         // 158
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 14, 15, -1, -1, -1, -1},           // 159
    {10, 11, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 160
    {0, 1, 10, 11, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 161
    {2, 3, 10, 11, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 162
    {0, 1, 2, 3, 10, 11, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},     // 163
    {4, 5, 10, 11, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 164
    {0, 1, 4, 5, 10, 11, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},     // 165
    {2, 3, 4, 5, 10, 11, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},     // 166
    {0, 1, 2, 3, 4, 5, 10, 11, 14, 15, -1, -1, -1, -1, -1, -1},       // 167
    {6, 7, 10, 11, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 168
    {0, 1, 6, 7, 10, 11, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},     // 169
    {2, 3, 6, 7, 10, 11, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},     // 170
    {0, 1, 2, 3, 6, 7, 10, 11, 14, 15, -1, -1, -1, -1, -1, -1},       // 171
    {4, 5, 6, 7, 10, 11, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},     // 172
    {0, 1, 4, 5, 6, 7, 10, 11, 14, 15, -1, -1, -1, -1, -1, -1},       // 173
    {2, 3, 4, 5, 6, 7, 10, 11, 14, 15, -1, -1, -1, -1, -1, -1},       // 174
    {0, 1, 2, 3, 4, 5, 6, 7, 10, 11, 14, 15, -1, -1, -1, -1},         // 175
    {8, 9, 10, 11, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 176
    {0, 1, 8, 9, 10, 11, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},     // 177
    {2, 3, 8, 9, 10, 11, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},     // 178
    {0, 1, 2, 3, 8, 9, 10, 11, 14, 15, -1, -1, -1, -1, -1, -1},       // 179
    {4, 5, 8, 9, 10, 11, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},     // 180
    {0, 1, 4, 5, 8, 9, 10, 11, 14, 15, -1, -1, -1, -1, -1, -1},       // 181
    {2, 3, 4, 5, 8, 9, 10, 11, 14, 15, -1, -1, -1, -1, -1, -1},       // 182
    {0, 1, 2, 3, 4, 5, 8, 9, 10, 11, 14, 15, -1, -1, -1, -1},         // 183
    {6, 7, 8, 9, 10, 11, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},     // 184
    {0, 1, 6, 7, 8, 9, 10, 11, 14, 15, -1, -1, -1, -1, -1, -1},       // 185
    {2, 3, 6, 7, 8, 9, 10, 11, 14, 15, -1, -1, -1, -1, -1, -1},       // 186
    {0, 1, 2, 3, 6, 7, 8, 9, 10, 11, 14, 15, -1, -1, -1, -1},         // 187
    {4, 5, 6, 7, 8, 9, 10, 11, 14, 15, -1, -1, -1, -1, -1, -1},       // 188
    {0, 1, 4, 5, 6, 7, 8, 9, 10, 11, 14, 15, -1, -1, -1, -1},         // 189
    {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 14, 15, -1, -1, -1, -1},         // 190
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 14, 15, -1, -1},           // 191
    {12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 192
    {0, 1, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 193
    {2, 3, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 194
    {0, 1, 2, 3, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},     // 195
    {4, 5, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 196
    {0, 1, 4, 5, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},     // 197
    {2, 3, 4, 5, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},     // 198
    {0, 1, 2, 3, 4, 5, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1},       // 199
    {6, 7, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 200
    {0, 1, 6, 7, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},     // 201
    {2, 3, 6, 7, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},     // 202
    {0, 1, 2, 3, 6, 7, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1},       // 203
    {4, 5, 6, 7, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},     // 204
    {0, 1, 4, 5, 6, 7, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1},       // 205
    {2, 3, 4, 5, 6, 7, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1},       // 206
    {0, 1, 2, 3, 4, 5, 6, 7, 12, 13, 14, 15, -1, -1, -1, -1},         // 207
    {8, 9, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   // 208
    {0, 1, 8, 9, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},     // 209
    {2, 3, 8, 9, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},     // 210
    {0, 1, 2, 3, 8, 9, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1},       // 211
    {4, 5, 8, 9, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},     // 212
    {0, 1, 4, 5, 8, 9, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1},       // 213
    {2, 3, 4, 5, 8, 9, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1},       // 214
    {0, 1, 2, 3, 4, 5, 8, 9, 12, 13, 14, 15, -1, -1, -1, -1},         // 215
    {6, 7, 8, 9, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},     // 216
    {0, 1, 6, 7, 8, 9, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1},       // 217
    {2, 3, 6, 7, 8, 9, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1},       // 218
    {0, 1, 2, 3, 6, 7, 8, 9, 12, 13, 14, 15, -1, -1, -1, -1},         // 219
    {4, 5, 6, 7, 8, 9, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1},       // 220
    {0, 1, 4, 5, 6, 7, 8, 9, 12, 13, 14, 15, -1, -1, -1, -1},         // 221
    {2, 3, 4, 5, 6, 7, 8, 9, 12, 13, 14, 15, -1, -1, -1, -1},         // 222
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 12, 13, 14, 15, -1, -1},           // 223
    {10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 224
    {0, 1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},   // 225
    {2, 3, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},   // 226
    {0, 1, 2, 3, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1},     // 227
    {4, 5, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},   // 228
    {0, 1, 4, 5, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1},     // 229
    {2, 3, 4, 5, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1},     // 230
    {0, 1, 2, 3, 4, 5, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1},       // 231
    {6, 7, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},   // 232
    {0, 1, 6, 7, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1},     // 233
    {2, 3, 6, 7, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1},     // 234
    {0, 1, 2, 3, 6, 7, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1},       // 235
    {4, 5, 6, 7, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1},     // 236
    {0, 1, 4, 5, 6, 7, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1},       // 237
    {2, 3, 4, 5, 6, 7, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1},       // 238
    {0, 1, 2, 3, 4, 5, 6, 7, 10, 11, 12, 13, 14, 15, -1, -1},         // 239
    {8, 9, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1},   // 240
    {0, 1, 8, 9, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1},     // 241
    {2, 3, 8, 9, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1},     // 242
    {0, 1, 2, 3, 8, 9, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1},       // 243
    {4, 5, 8, 9, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1},     // 244
    {0, 1, 4, 5, 8, 9, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1},       // 245
    {2, 3, 4, 5, 8, 9, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1},       // 246
    {0, 1, 2, 3, 4, 5, 8, 9, 10, 11, 12, 13, 14, 15, -1, -1},         // 247
    {6, 7, 8, 9, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1},     // 248
    {0, 1, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1},       // 249
    {2, 3, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1},       // 250
    {0, 1, 2, 3, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, -1, -1},         // 251
    {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1},       // 252
    {0, 1, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, -1, -1},         // 253
    {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, -1, -1},         // 254
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},           // 255
};

const uint8_t table_idx_small[16][8] = {
    {-1, -1, -1, -1, -1, -1, -1, -1}, // 0
    {0, 1, -1, -1, -1, -1, -1, -1},   // 1
    {2, 3, -1, -1, -1, -1, -1, -1},   // 2
    {0, 1, 2, 3, -1, -1, -1, -1},     // 3
    {4, 5, -1, -1, -1, -1, -1, -1},   // 4
    {0, 1, 4, 5, -1, -1, -1, -1},     // 5
    {2, 3, 4, 5, -1, -1, -1, -1},     // 6
    {0, 1, 2, 3, 4, 5, -1, -1},       // 7
    {6, 7, -1, -1, -1, -1, -1, -1},   // 8
    {0, 1, 6, 7, -1, -1, -1, -1},     // 9
    {2, 3, 6, 7, -1, -1, -1, -1},     // 10
    {0, 1, 2, 3, 6, 7, -1, -1},       // 11
    {4, 5, 6, 7, -1, -1, -1, -1},     // 12
    {0, 1, 4, 5, 6, 7, -1, -1},       // 13
    {2, 3, 4, 5, 6, 7, -1, -1},       // 14
    {0, 1, 2, 3, 4, 5, 6, 7},         // 15
};

void print_array(const uint8_t *buf, int buflen, const char *string)
{
    printf("%s: [", string);
    for (int i = 0; i < buflen; i++)
    {
        printf("%d, ", buf[i]);
    }
    printf("]\n");
}

/*
This function is taken from 
https://github.com/pq-crystals/kyber/blob/master/ref/indcpa.c#L124
*/
unsigned int rej_uniform(int16_t *r,
                         unsigned int len,
                         const uint8_t *buf,
                         const unsigned int buflen)
{
#ifdef DEBUG
    print_array(buf, buflen, "ref_buf");
#endif
    unsigned int ctr, pos;
    uint16_t val0, val1;

    ctr = pos = 0;
    while (ctr < len && pos + 3 <= buflen)
    {
        val0 = ((buf[pos + 0] >> 0) | ((uint16_t)buf[pos + 1] << 8)) & 0xFFF;
        val1 = ((buf[pos + 1] >> 4) | ((uint16_t)buf[pos + 2] << 4)) & 0xFFF;
        pos += 3;

        if (val0 < 3329)
            r[ctr++] = val0;
        if (ctr < len && val1 < 3329)
            r[ctr++] = val1;
    }

    ctr = ctr > 256 ? 256 : ctr;
    return ctr;
}

int compare(int16_t *a, int16_t *b, int len)
{
    int count = 0;
    for (int i = 0; i < len; i++)
    {
        if (a[i] != b[i])
        {
            printf("[%d] %d != %d\n", i, a[i], b[i]);
            count += 1;
        }
        if (count > 8)
        {
            return 1;
        }
    }
    if (count)
    {
        return 1;
    }
    return 0;
}

unsigned int neon_rej_uniform(int16_t *r, const uint8_t *buf)
{
#ifdef DEBUG
    print_array(buf, BUF_LEN, "neon_buf");
#endif

    uint8x16x3_t neon_buf;
    uint16x8x4_t tmp, value, sign;

    uint16_t bit_table[8] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80};
    uint8x16x4_t neon_table;
    uint16x8_t const_kyberq, neon_bit, const_0xfff;

    const_0xfff = vdupq_n_u16(0xfff);
    const_kyberq = vdupq_n_u16(3329 - 1);
    neon_bit = vld1q_u16(bit_table);

    unsigned int reduce_indexes[4], ctr[4];
    unsigned int i, count = 0;

    int16_t local_buf[BUF_LEN * 8 / 12];

    for (i = 0; i < BUF_LEN && count < 256; i += 16 * 3)
    {
        // 0, 3, 6, 9  - 12, 15, 18, 21
        // 1, 4, 7, 10 - 13, 16, 19, 22
        // 2, 5, 8, 11 - 14, 17, 20, 23
        neon_buf = vld3q_u8(&buf[i]);

        // Val0: 0-1 | 3-4 | 6-7| 9-10 | 12-13 | 15-16 | 18-19 | 21-22
        tmp.val[0] = (uint16x8_t)vzip1q_u8(neon_buf.val[0], neon_buf.val[1]);
        tmp.val[1] = (uint16x8_t)vzip2q_u8(neon_buf.val[0], neon_buf.val[1]);

        // Take 12 bit low
        tmp.val[0] = vandq_u16(tmp.val[0], const_0xfff);
        tmp.val[1] = vandq_u16(tmp.val[1], const_0xfff);

        // Val1: 1-2 | 4-5 | 7-8 | 10-11 | 13-14 | 16-17 | 19-20 | 22-23
        tmp.val[2] = (uint16x8_t)vzip1q_u8(neon_buf.val[1], neon_buf.val[2]);
        tmp.val[3] = (uint16x8_t)vzip2q_u8(neon_buf.val[1], neon_buf.val[2]);

        // Take 12 bit high
        tmp.val[2] = vshrq_n_u16(tmp.val[2], 4);
        tmp.val[3] = vshrq_n_u16(tmp.val[3], 4);

        // Final value
        // Index as below:
        // 0, 1, 2, 3 -- 4, 5, 6, 7
        // 8, 9, 10, 11 -- 12, 13, 14, 15
        // 16, 17, 18, 19 -- 20, 21, 22, 23
        // 24, 25, 26, 27 -- 28, 29, 30, 31
        value.val[0] = vzip1q_u16(tmp.val[0], tmp.val[2]);
        value.val[1] = vzip2q_u16(tmp.val[0], tmp.val[2]);
        value.val[2] = vzip1q_u16(tmp.val[1], tmp.val[3]);
        value.val[3] = vzip2q_u16(tmp.val[1], tmp.val[3]);

        // Compare unsigned less than equal than constant
        sign.val[0] = vcleq_u16(value.val[0], const_kyberq);
        sign.val[1] = vcleq_u16(value.val[1], const_kyberq);
        sign.val[2] = vcleq_u16(value.val[2], const_kyberq);
        sign.val[3] = vcleq_u16(value.val[3], const_kyberq);

        // Encoding: prepare indexes for table idx
        sign.val[0] = vandq_u16(sign.val[0], neon_bit);
        sign.val[1] = vandq_u16(sign.val[1], neon_bit);
        sign.val[2] = vandq_u16(sign.val[2], neon_bit);
        sign.val[3] = vandq_u16(sign.val[3], neon_bit);

        // Add across vector
        reduce_indexes[0] = vaddvq_u16(sign.val[0]);
        reduce_indexes[1] = vaddvq_u16(sign.val[1]);
        reduce_indexes[2] = vaddvq_u16(sign.val[2]);
        reduce_indexes[3] = vaddvq_u16(sign.val[3]);

        ctr[0] = __builtin_popcount(reduce_indexes[0]);
        ctr[1] = __builtin_popcount(reduce_indexes[1]);
        ctr[2] = __builtin_popcount(reduce_indexes[2]);
        ctr[3] = __builtin_popcount(reduce_indexes[3]);

        neon_table.val[0] = vld1q_u8(table_idx[reduce_indexes[0]]);
        neon_table.val[1] = vld1q_u8(table_idx[reduce_indexes[1]]);
        neon_table.val[2] = vld1q_u8(table_idx[reduce_indexes[2]]);
        neon_table.val[3] = vld1q_u8(table_idx[reduce_indexes[3]]);

        // Table-based permute for each array
        value.val[0] = (uint16x8_t)vqtbl1q_u8((uint8x16_t)value.val[0], neon_table.val[0]);
        value.val[1] = (uint16x8_t)vqtbl1q_u8((uint8x16_t)value.val[1], neon_table.val[1]);
        value.val[2] = (uint16x8_t)vqtbl1q_u8((uint8x16_t)value.val[2], neon_table.val[2]);
        value.val[3] = (uint16x8_t)vqtbl1q_u8((uint8x16_t)value.val[3], neon_table.val[3]);

        vst1q_s16(&local_buf[count], (int16x8_t)value.val[0]);
        count += ctr[0];
        vst1q_s16(&local_buf[count], (int16x8_t)value.val[1]);
        count += ctr[1];
        vst1q_s16(&local_buf[count], (int16x8_t)value.val[2]);
        count += ctr[2];
        vst1q_s16(&local_buf[count], (int16x8_t)value.val[3]);
        count += ctr[3];
    }

    count = count > 256 ? 256 : count;

    // Copy from localbuf to output, make sure no overflow happen
    for (i = 0; i < count; i++)
    {
        r[i] = local_buf[i];
    }

    return count;
}

/*
This function is similar to neon_rej_uniform, 
but only operate on half of SIMD register

The purpose is to use smaller table.
*/

unsigned int neon_rej_uniform_half(int16_t *r, const uint8_t *buf)
{
#ifdef DEBUG
    print_array(buf, BUF_LEN, "neon_buf_half");
#endif
    uint8x8x3_t neon_buf8;
    uint16x4x4_t tmp8, value8, sign8;
    uint8x8x4_t neon_table8;
    uint16x4_t const8_kyberq, neon_bit8, const8_0xfff;

    uint16_t bit_table[8] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80};
    unsigned int reduce_indexes[4], ctr[4];

    const8_0xfff = vdup_n_u16(0xfff);
    const8_kyberq = vdup_n_u16(3329 - 1);
    neon_bit8 = vld1_u16(bit_table);

    int16_t local_buf[BUF_LEN * 8 / 12];
    int i, count = 0;

    for (i = 0; i < BUF_LEN && count < 256; i += 8 * 3)
    {
        neon_buf8 = vld3_u8(&buf[i]);

        // Val0: 0-1 | 3-4 | 6-7| 9-10
        tmp8.val[0] = (uint16x4_t)vzip1_u8(neon_buf8.val[0], neon_buf8.val[1]);
        tmp8.val[1] = (uint16x4_t)vzip2_u8(neon_buf8.val[0], neon_buf8.val[1]);

        tmp8.val[0] = vand_u16(tmp8.val[0], const8_0xfff);
        tmp8.val[1] = vand_u16(tmp8.val[1], const8_0xfff);

        // Val1: 1-2 | 4-5 | 7-8 | 10-11
        tmp8.val[2] = (uint16x4_t)vzip1_u8(neon_buf8.val[1], neon_buf8.val[2]);
        tmp8.val[3] = (uint16x4_t)vzip2_u8(neon_buf8.val[1], neon_buf8.val[2]);

        tmp8.val[2] = vshr_n_u16(tmp8.val[2], 4);
        tmp8.val[3] = vshr_n_u16(tmp8.val[3], 4);

        // Final value
        value8.val[0] = vzip1_u16(tmp8.val[0], tmp8.val[2]);
        value8.val[1] = vzip2_u16(tmp8.val[0], tmp8.val[2]);
        value8.val[2] = vzip1_u16(tmp8.val[1], tmp8.val[3]);
        value8.val[3] = vzip2_u16(tmp8.val[1], tmp8.val[3]);

        // Compare unsigned less than equal
        sign8.val[0] = vcle_u16(value8.val[0], const8_kyberq);
        sign8.val[1] = vcle_u16(value8.val[1], const8_kyberq);
        sign8.val[2] = vcle_u16(value8.val[2], const8_kyberq);
        sign8.val[3] = vcle_u16(value8.val[3], const8_kyberq);

        // Prepare indexes for table idx
        sign8.val[0] = vand_u16(sign8.val[0], neon_bit8);
        sign8.val[1] = vand_u16(sign8.val[1], neon_bit8);
        sign8.val[2] = vand_u16(sign8.val[2], neon_bit8);
        sign8.val[3] = vand_u16(sign8.val[3], neon_bit8);

        // Add across vector
        reduce_indexes[0] = vaddv_u16(sign8.val[0]);
        reduce_indexes[1] = vaddv_u16(sign8.val[1]);
        reduce_indexes[2] = vaddv_u16(sign8.val[2]);
        reduce_indexes[3] = vaddv_u16(sign8.val[3]);

        // Popcount
        ctr[0] = __builtin_popcount(reduce_indexes[0]);
        ctr[1] = __builtin_popcount(reduce_indexes[1]);
        ctr[2] = __builtin_popcount(reduce_indexes[2]);
        ctr[3] = __builtin_popcount(reduce_indexes[3]);

        // Load entry for permutation
        neon_table8.val[0] = vld1_u8(table_idx_small[reduce_indexes[0]]);
        neon_table8.val[1] = vld1_u8(table_idx_small[reduce_indexes[1]]);
        neon_table8.val[2] = vld1_u8(table_idx_small[reduce_indexes[2]]);
        neon_table8.val[3] = vld1_u8(table_idx_small[reduce_indexes[3]]);

        // Table-based permute
        value8.val[0] = (uint16x4_t)vtbl1_u8((uint8x8_t)value8.val[0], neon_table8.val[0]);
        value8.val[1] = (uint16x4_t)vtbl1_u8((uint8x8_t)value8.val[1], neon_table8.val[1]);
        value8.val[2] = (uint16x4_t)vtbl1_u8((uint8x8_t)value8.val[2], neon_table8.val[2]);
        value8.val[3] = (uint16x4_t)vtbl1_u8((uint8x8_t)value8.val[3], neon_table8.val[3]);

        vst1_s16(&local_buf[count], (int16x4_t)value8.val[0]);
        count += ctr[0];
        vst1_s16(&local_buf[count], (int16x4_t)value8.val[1]);
        count += ctr[1];
        vst1_s16(&local_buf[count], (int16x4_t)value8.val[2]);
        count += ctr[2];
        vst1_s16(&local_buf[count], (int16x4_t)value8.val[3]);
        count += ctr[3];
    }

    count = count > 256 ? 256 : count;

    for (i = 0; i < count; i++)
    {
        r[i] = local_buf[i];
    }
    return count;
}

unsigned int neon_rej_uniform_mix(int16_t *r, const uint8_t *buf)
{
    uint8x16x3_t neon_buf;
    uint16x8x4_t tmp, value, sign;

    uint16_t bit_table[8] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80};
    uint8x16x4_t neon_table;
    uint16x8_t const_kyberq, neon_bit, const_0xfff;

    neon_bit = vld1q_u16(bit_table);
    const_kyberq = vdupq_n_u16(3329 - 1);
    const_0xfff = vdupq_n_u16(0xfff);

    unsigned int reduce_indexes[4], ctr[4];
    unsigned int i = 0, count = 0;

    for (i = 0; i < (BUF_LEN - 48) && count < (256 - 32); i += 16 * 3)
    {
        // 0, 3, 6, 9  - 12, 15, 18, 21
        // 1, 4, 7, 10 - 13, 16, 19, 22
        // 2, 5, 8, 11 - 14, 17, 20, 23
        neon_buf = vld3q_u8(&buf[i]);

        // Val0: 0-1 | 3-4 | 6-7| 9-10 | 12-13 | 15-16 | 18-19 | 21-22
        tmp.val[0] = (uint16x8_t)vzip1q_u8(neon_buf.val[0], neon_buf.val[1]);
        tmp.val[1] = (uint16x8_t)vzip2q_u8(neon_buf.val[0], neon_buf.val[1]);

        // Take 12 bit low
        tmp.val[0] = vandq_u16(tmp.val[0], const_0xfff);
        tmp.val[1] = vandq_u16(tmp.val[1], const_0xfff);

        // Val1: 1-2 | 4-5 | 7-8 | 10-11 | 13-14 | 16-17 | 19-20 | 22-23
        tmp.val[2] = (uint16x8_t)vzip1q_u8(neon_buf.val[1], neon_buf.val[2]);
        tmp.val[3] = (uint16x8_t)vzip2q_u8(neon_buf.val[1], neon_buf.val[2]);

        // Take 12 bit high
        tmp.val[2] = vshrq_n_u16(tmp.val[2], 4);
        tmp.val[3] = vshrq_n_u16(tmp.val[3], 4);

        // Final value
        // Index as below:
        // 0, 1, 2, 3 -- 4, 5, 6, 7
        // 8, 9, 10, 11 -- 12, 13, 14, 15
        // 16, 17, 18, 19 -- 20, 21, 22, 23
        // 24, 25, 26, 27 -- 28, 29, 30, 31
        value.val[0] = vzip1q_u16(tmp.val[0], tmp.val[2]);
        value.val[1] = vzip2q_u16(tmp.val[0], tmp.val[2]);
        value.val[2] = vzip1q_u16(tmp.val[1], tmp.val[3]);
        value.val[3] = vzip2q_u16(tmp.val[1], tmp.val[3]);

        // Compare unsigned less than equal than constant
        sign.val[0] = vcleq_u16(value.val[0], const_kyberq);
        sign.val[1] = vcleq_u16(value.val[1], const_kyberq);
        sign.val[2] = vcleq_u16(value.val[2], const_kyberq);
        sign.val[3] = vcleq_u16(value.val[3], const_kyberq);

        // Encoding: prepare indexes for table idx
        sign.val[0] = vandq_u16(sign.val[0], neon_bit);
        sign.val[1] = vandq_u16(sign.val[1], neon_bit);
        sign.val[2] = vandq_u16(sign.val[2], neon_bit);
        sign.val[3] = vandq_u16(sign.val[3], neon_bit);

        // Add across vector
        reduce_indexes[0] = vaddvq_u16(sign.val[0]);
        reduce_indexes[1] = vaddvq_u16(sign.val[1]);
        reduce_indexes[2] = vaddvq_u16(sign.val[2]);
        reduce_indexes[3] = vaddvq_u16(sign.val[3]);

        ctr[0] = __builtin_popcount(reduce_indexes[0]);
        ctr[1] = __builtin_popcount(reduce_indexes[1]);
        ctr[2] = __builtin_popcount(reduce_indexes[2]);
        ctr[3] = __builtin_popcount(reduce_indexes[3]);

        neon_table.val[0] = vld1q_u8(table_idx[reduce_indexes[0]]);
        neon_table.val[1] = vld1q_u8(table_idx[reduce_indexes[1]]);
        neon_table.val[2] = vld1q_u8(table_idx[reduce_indexes[2]]);
        neon_table.val[3] = vld1q_u8(table_idx[reduce_indexes[3]]);

        // Table-based permute for each array
        value.val[0] = (uint16x8_t)vqtbl1q_u8((uint8x16_t)value.val[0], neon_table.val[0]);
        value.val[1] = (uint16x8_t)vqtbl1q_u8((uint8x16_t)value.val[1], neon_table.val[1]);
        value.val[2] = (uint16x8_t)vqtbl1q_u8((uint8x16_t)value.val[2], neon_table.val[2]);
        value.val[3] = (uint16x8_t)vqtbl1q_u8((uint8x16_t)value.val[3], neon_table.val[3]);

        vst1q_s16(&r[count], (int16x8_t)value.val[0]);
        count += ctr[0];
        vst1q_s16(&r[count], (int16x8_t)value.val[1]);
        count += ctr[1];
        vst1q_s16(&r[count], (int16x8_t)value.val[2]);
        count += ctr[2];
        vst1q_s16(&r[count], (int16x8_t)value.val[3]);
        count += ctr[3];
    }

    uint8x8x3_t neon_buf8;
    uint16x4x4_t tmp8, value8, sign8;
    uint8x8x4_t neon_table8;
    uint16x4_t const8_kyberq, neon_bit8, const8_0xfff;
    const8_kyberq = vdup_n_u16(3329 - 1);
    const8_0xfff = vdup_n_u16(0xfff);
    neon_bit8 = vld1_u16(bit_table);

    int16_t local_buf[48], local_index = 0;
    unsigned int bound = count;
#ifdef DEBUG
    printf("count = %d\n", count);
#endif

    do
    {
        neon_buf8 = vld3_u8(&buf[i]);

        // Val0: 0-1 | 3-4 | 6-7| 9-10
        tmp8.val[0] = (uint16x4_t)vzip1_u8(neon_buf8.val[0], neon_buf8.val[1]);
        tmp8.val[1] = (uint16x4_t)vzip2_u8(neon_buf8.val[0], neon_buf8.val[1]);

        tmp8.val[0] = vand_u16(tmp8.val[0], const8_0xfff);
        tmp8.val[1] = vand_u16(tmp8.val[1], const8_0xfff);

        // Val1: 1-2 | 4-5 | 7-8 | 10-11
        tmp8.val[2] = (uint16x4_t)vzip1_u8(neon_buf8.val[1], neon_buf8.val[2]);
        tmp8.val[3] = (uint16x4_t)vzip2_u8(neon_buf8.val[1], neon_buf8.val[2]);

        tmp8.val[2] = vshr_n_u16(tmp8.val[2], 4);
        tmp8.val[3] = vshr_n_u16(tmp8.val[3], 4);

        // Final value
        value8.val[0] = vzip1_u16(tmp8.val[0], tmp8.val[2]);
        value8.val[1] = vzip2_u16(tmp8.val[0], tmp8.val[2]);
        value8.val[2] = vzip1_u16(tmp8.val[1], tmp8.val[3]);
        value8.val[3] = vzip2_u16(tmp8.val[1], tmp8.val[3]);

        // Compare unsigned less than equal
        sign8.val[0] = vcle_u16(value8.val[0], const8_kyberq);
        sign8.val[1] = vcle_u16(value8.val[1], const8_kyberq);
        sign8.val[2] = vcle_u16(value8.val[2], const8_kyberq);
        sign8.val[3] = vcle_u16(value8.val[3], const8_kyberq);

        // Prepare indexes for table idx
        sign8.val[0] = vand_u16(sign8.val[0], neon_bit8);
        sign8.val[1] = vand_u16(sign8.val[1], neon_bit8);
        sign8.val[2] = vand_u16(sign8.val[2], neon_bit8);
        sign8.val[3] = vand_u16(sign8.val[3], neon_bit8);

        // Add across vector
        reduce_indexes[0] = vaddv_u16(sign8.val[0]);
        reduce_indexes[1] = vaddv_u16(sign8.val[1]);
        reduce_indexes[2] = vaddv_u16(sign8.val[2]);
        reduce_indexes[3] = vaddv_u16(sign8.val[3]);

        // Popcount
        ctr[0] = __builtin_popcount(reduce_indexes[0]);
        ctr[1] = __builtin_popcount(reduce_indexes[1]);
        ctr[2] = __builtin_popcount(reduce_indexes[2]);
        ctr[3] = __builtin_popcount(reduce_indexes[3]);

        // Load entry for permutation
        neon_table8.val[0] = vld1_u8(table_idx_small[reduce_indexes[0]]);
        neon_table8.val[1] = vld1_u8(table_idx_small[reduce_indexes[1]]);
        neon_table8.val[2] = vld1_u8(table_idx_small[reduce_indexes[2]]);
        neon_table8.val[3] = vld1_u8(table_idx_small[reduce_indexes[3]]);

        // Table-based permute
        value8.val[0] = (uint16x4_t)vtbl1_u8((uint8x8_t)value8.val[0], neon_table8.val[0]);
        value8.val[1] = (uint16x4_t)vtbl1_u8((uint8x8_t)value8.val[1], neon_table8.val[1]);
        value8.val[2] = (uint16x4_t)vtbl1_u8((uint8x8_t)value8.val[2], neon_table8.val[2]);
        value8.val[3] = (uint16x4_t)vtbl1_u8((uint8x8_t)value8.val[3], neon_table8.val[3]);

        vst1_s16(&local_buf[local_index], (int16x4_t)value8.val[0]);
        local_index += ctr[0];
        vst1_s16(&local_buf[local_index], (int16x4_t)value8.val[1]);
        local_index += ctr[1];
        vst1_s16(&local_buf[local_index], (int16x4_t)value8.val[2]);
        local_index += ctr[2];
        vst1_s16(&local_buf[local_index], (int16x4_t)value8.val[3]);
        local_index += ctr[3];

        bound = count + local_index;
#ifdef DEBUG
        printf("bound = %d, i = %d, local_index = %d\n", bound, i, local_index);
#endif
        i += 8 * 3;
    } while ((bound < 256) && (i < BUF_LEN));

    bound = bound > 256 ? 256 : bound;

    for (i = 0; count < bound; i++, count++)
    {
        r[count] = local_buf[i];
#ifdef DEBUG
        printf("local_buf[%d]: %d: %d\n", i, count, local_buf[i]);
#endif
    }

    return count;
}
