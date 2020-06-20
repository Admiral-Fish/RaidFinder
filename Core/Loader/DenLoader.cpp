/*
 * This file is part of RaidFinder
 * Copyright (C) 2019-2020 by Admiral_Fish
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "DenLoader.hpp"
#include <QApplication>
#include <QFile>
#include <QHash>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <array>

// Normal hash, rare hash, location, x, y
// clang-format off
constexpr u64 denInfo[190][5] =
    {
        { 0x173f0456dc5dfc52, 0xba83e1671012ebcd, 12, 185, 977 }, // 16 52  ; 0
        { 0x17458556dc634333, 0xba8745671015cb90, 12, 125, 1005 }, // 37 64
        { 0x17458b56dc634d65, 0x450421d99cf882c1, 12, 114, 936 }, // 31 90
        { 0x17428156dc610690, 0xba805467100fc65f, 12, 311, 998 }, // 3 51
        { 0x17428856dc611275, 0xba805767100fcb78, 12, 233, 948 }, // 4 46
        { 0x17458956dc6349ff, 0xba8747671015cef6, 12, 337, 996 }, // 33 62
        { 0x17459356dc635afd, 0xba8746671015cd43, 12, 209, 976 }, // 39 65
        { 0x17428356dc6109f6, 0xba805967100fcede, 12, 136, 906 }, // 1 48
        { 0x17458b56dc634d65, 0x450421d99cf882c1, 12, 252, 905 }, // 31 90
        { 0x17491656dc666f6d, 0xba83da671012dfe8, 2, 30, 927 }, // 26 59
        { 0x17490856dc6657a3, 0x4500a2d99cf5751d, 2, 12, 851 }, // 28 79    ; 10
        { 0x17491656dc666f6d, 0xba83db671012e19b, 2, 12, 861 }, // 26 58
        { 0x17428856dc611275, 0x45041fd99cf87f5b, 2, 20, 913 }, // 4 92
        { 0x17491656dc666f6d, 0xba83da671012dfe8, 2, 40, 878 }, // 26 59
        { 0x17428256dc610843, 0xba805367100fc4ac, 15, 52, 776 }, // 2 50
        { 0x17428656dc610f0f, 0xba805867100fcd2b, 15, 68, 652 }, // 6 47
        { 0x0000000000000000, 0x0000000000000000, 15, 50, 700 }, // Special
        { 0x17428556dc610d5c, 0xba805d67100fd5aa, 4, 217, 913 }, // 7 44
        { 0x17459356dc635afd, 0xba8746671015cd43, 4, 158, 705 }, // 39 65
        { 0x17458b56dc634d65, 0xba83d8671012dc82, 4, 220, 759 }, // 31 61
        { 0x17458b56dc634d65, 0xba83d8671012dc82, 4, 248, 852 }, // 31 61   ; 20
        { 0x17428b56dc61178e, 0xba8a4e67101810b2, 16, 129, 818 }, // 9 75
        { 0x17428b56dc61178e, 0xba8a4e67101810b2, 16, 131, 735 }, // 9 75
        { 0x17501a56dc6c94e7, 0xba805d67100fd5aa, 16, 105, 907 }, // 44 42
        { 0x17428556dc610d5c, 0xba805d67100fd5aa, 16, 50, 909 }, // 7 44
        { 0x17428b56dc61178e, 0x450420d99cf8810e, 16, 98, 750 }, // 9 91
        { 0x17458756dc634699, 0xba8748671015d0a9, 16, 107, 652 }, // 35 63
        { 0x17459256dc63594a, 0xba8745671015cb90, 0, 186, 816 }, // 38 64
        { 0x17428c56dc611941, 0x450420d99cf8810e, 13, 310, 824 }, // 8 91
        { 0x17501b56dc6c969a, 0xba8a496710180833, 13, 359, 980 }, // 41 76
        { 0x17501b56dc6c969a, 0xba8a496710180833, 13, 393, 962 }, // 41 76  ; 30
        { 0x17428756dc6110c2, 0xba805767100fcb78, 13, 328, 761 }, // 5 46
        { 0x17428356dc6109f6, 0xba805c67100fd3f7, 13, 352, 765 }, // 1 43
        { 0x173f0356dc5dfa9f, 0xba805467100fc65f, 7, 443, 895 }, // 15 51
        { 0x173f0056dc5df586, 0xba805e67100fd75d, 7, 388, 817 }, // 12 45
        { 0x173eff56dc5df3d3, 0xba805a67100fd091, 7, 443, 830 }, // 11 49
        { 0x173f0356dc5dfa9f, 0x45009ed99cf56e51, 7, 410, 952 }, // 15 83
        { 0x173eff56dc5df3d3, 0x450097d99cf5626c, 7, 447, 815 }, // 11 84
        { 0x173efe56dc5df220, 0xba805967100fcede, 11, 393, 781 }, // 10 48
        { 0x17501b56dc6c969a, 0xba8a496710180833, 11, 314, 755 }, // 41 76
        { 0x17490756dc6655f0, 0xba83d9671012de35, 11, 440, 658 }, // 29 60  ; 40
        { 0x17501b56dc6c969a, 0xba8a496710180833, 11, 281, 717 }, // 41 76
        { 0x17490756dc6655f0, 0xba83d9671012de35, 11, 440, 703 }, // 29 60
        { 0x17490756dc6655f0, 0x450425d99cf8898d, 4, 310, 654 }, // 29 86
        { 0x173efe56dc5df220, 0xba805967100fcede, 11, 443, 792 }, // 10 48
        { 0x173f0256dc5df8ec, 0xba805367100fc4ac, 10, 412, 533 }, // 14 50
        { 0x17458a56dc634bb2, 0xba83d9671012de35, 10, 345, 537 }, // 30 60
        { 0x17491456dc666c07, 0xba83dd671012e501, 10, 365, 559 }, // 24 56
        { 0x17501c56dc6c984d, 0xba8746671015cd43, 10, 408, 570 }, // 40 65
        { 0x17458656dc6344e6, 0x45009dd99cf56c9e, 1, 193, 295 }, // 34 82
        { 0x173f0156dc5df739, 0x450424d99cf887da, 1, 274, 321 }, // 13 87   ; 50
        { 0x17428c56dc611941, 0xba805d67100fd5aa, 1, 328, 330 }, // 8 44
        { 0x173f0456dc5dfc52, 0xba83e1671012ebcd, 1, 370, 452 }, // 16 52
        { 0x17501c56dc6c984d, 0xba8746671015cd43, 1, 224, 282 }, // 40 65
        { 0x17428856dc611275, 0x45041fd99cf87f5b, 1, 342, 312 }, // 4 92
        { 0x17428756dc6110c2, 0xba805767100fcb78, 1, 340, 269 }, // 5 46
        { 0x17458456dc634180, 0xba8748671015d0a9, 1, 305, 323 }, // 36 63
        { 0x17491556dc666dba, 0xba83da671012dfe8, 1, 334, 368 }, // 27 59
        { 0x17501c56dc6c984d, 0xba874b671015d5c2, 14, 343, 222 }, // 40 66
        { 0x17428356dc6109f6, 0x45009cd99cf56aeb, 14, 348, 195 }, // 1 81
        { 0x17428156dc610690, 0xba805467100fc65f, 14, 200, 244 }, // 3 51   ; 60
        { 0x173f0756dc5e016b, 0x45009bd99cf56938, 14, 305, 183 }, // 19 80
        { 0x17428656dc610f0f, 0xba805c67100fd3f7, 14, 348, 180 }, // 6 43
        { 0x17491556dc666dba, 0xba83da671012dfe8, 14, 260, 199 }, // 27 59
        { 0x17428256dc610843, 0xba805367100fc4ac, 14, 211, 205 }, // 2 50
        { 0x173f0056dc5df586, 0x450098d99cf5641f, 14, 303, 242 }, // 12 85
        { 0x17491256dc6668a1, 0xba83de671012e6b4, 14, 324, 209 }, // 22 55
        { 0x173f0256dc5df8ec, 0xba805367100fc4ac, 14, 326, 219 }, // 14 50
        { 0x173f0656dc5dffb8, 0xba83df671012e867, 14, 330, 215 }, // 18 54
        { 0x17458756dc634699, 0xba8748671015d0a9, 14, 282, 195 }, // 35 63
        { 0x173f0556dc5dfe05, 0x45041ed99cf87da8, 3, 265, 139 }, // 17 93   ; 70
        { 0x173f0556dc5dfe05, 0xba83e1671012ebcd, 3, 307, 93 }, // 17 52
        { 0x173f0356dc5dfa9f, 0x45041ed99cf87da8, 3, 331, 84 }, // 15 93
        { 0x17428b56dc61178e, 0xba8a4e67101810b2, 3, 219, 99 }, // 9 75
        { 0x173eff56dc5df3d3, 0xba805a67100fd091, 3, 243, 120 }, // 11 49
        { 0x173efe56dc5df220, 0xba805967100fcede, 3, 262, 174 }, // 10 48
        { 0x17490f56dc666388, 0xba83de671012e6b4, 3, 283, 98 }, // 21 55
        { 0x17491056dc66653b, 0xba83df671012e867, 3, 304, 112 }, // 20 54
        { 0x17490856dc6657a3, 0xba805c67100fd3f7, 6, 306, 145 }, // 28 43
        { 0x17458456dc634180, 0x450423d99cf88627, 3, 214, 168 }, // 36 88
        { 0x17501a56dc6c94e7, 0xba874c671015d775, 6, 334, 145 }, // 42 67   ; 80
        { 0x17458456dc634180, 0xba874a671015d40f, 6, 347, 103 }, // 36 69
        { 0x17491556dc666dba, 0xba874f671015dc8e, 6, 363, 88 }, // 27 70
        { 0x17491356dc666a54, 0xba8a4d6710180eff, 6, 338, 122 }, // 25 72
        { 0x173f0056dc5df586, 0xba805e67100fd75d, 8, 339, 35 }, // 12 45
        { 0x17458856dc63484c, 0xba83d8671012dc82, 8, 310, 65 }, // 32 61
        { 0x17458a56dc634bb2, 0xba83dc671012e34e, 8, 202, 34 }, // 30 57
        { 0x173f0756dc5e016b, 0xba83e0671012ea1a, 8, 237, 67 }, // 19 53
        { 0x17491156dc6666ee, 0xba8a4c6710180d4c, 8, 183, 47 }, // 23 73
        { 0x17458956dc6349ff, 0xba8747671015cef6, 8, 221, 50 }, // 33 62
        { 0x173f0256dc5df8ec, 0xba8749671015d25c, 8, 354, 60 }, // 14 68    ; 90
        { 0x17458a56dc634bb2, 0xba83d9671012de35, 5, 181, 185 }, // 30 60
        { 0x17491156dc6666ee, 0xba83de671012e6b4, 5, 199, 145 }, // 23 55
        { 0x173f0656dc5dffb8, 0xba8750671015de41, 5, 193, 173 }, // 18 71
        { 0x17458856dc63484c, 0x450422d99cf88474, 5, 202, 95 }, // 32 89
        { 0x17491456dc666c07, 0x4500a1d99cf5736a, 5, 185, 135 }, // 24 78
        { 0x17491356dc666a54, 0xba83dd671012e501, 9, 170, 35 }, // 25 56
        { 0x173f0656dc5dffb8, 0x4500a0d99cf571b7, 9, 128, 58 }, // 18 77
        { 0x17428c56dc611941, 0xba805d67100fd5aa, 9, 161, 80 }, // 8 44
        { 0x17458656dc6344e6, 0xba8a4f6710181265, 9, 143, 39 }, // 34 74
        { 0x79b25a4f80255a38, 0xc8ea8c1618ab0a58, 17, 643, 822}, // 100
        { 0xe2c6e5e725342f4a, 0x89955cc3a594e51a, 17, 770, 794},
        { 0x6d015b7858eb5119, 0x53441b80e563ef1f, 17, 723, 812},
        { 0x4257e50e1c471596, 0xfe9697f9799c65be, 17, 862, 770},
        { 0x2998f2424d0353eb, 0xae57b2a84974c3a1, 17, 673, 766},
        { 0x5b72bfac0ff3f885, 0x316e6b5e74bc7aa3, 17, 882, 745},
        { 0x21f6c965b3513d5e, 0xd8f100cde5822516, 17, 661, 838},
        { 0x6e6b46639f77f0c8, 0x1c1962488c012ee8, 17, 683, 792},
        { 0xbc3d01fff751cde4, 0x6f948f09933cdfc, 17, 831, 770},
        { 0x4257e30e1c471230, 0xfe9695f9799c6258, 17, 727, 779},
        { 0x4257e40e1c4713e3, 0xfe9696f9799c640b, 18, 662, 681}, // 110
        { 0x2998f2424d0353eb, 0xae57b2a84974c3a1, 18, 741, 680},
        { 0xe2c6e5e725342f4a, 0x89955cc3a594e51a, 18, 697, 645},
        { 0xc63dec8a65b5c540, 0x6aebee2a2d6d8470, 18, 732, 631},
        { 0x5c9a35ca819b38c8, 0xf9222e1acdf486e8, 18, 634, 623},
        { 0xb8a5e528bfee71bc, 0xdf017f3fefba2704, 18, 603, 591},
        { 0x21f6c865b3513bab, 0xd8f0ffcde5822363, 18, 667, 614},
        { 0x21f6c965b3513d5e, 0xd8f100cde5822516, 18, 609, 668},
        { 0x4257e50e1c471596, 0xfe9697f9799c65be, 18, 554, 577},
        { 0x6d015b7858eb5119, 0x53441b80e563ef1f, 19, 533, 524},
        { 0xdb8629cba3383296, 0x4f1e561dd73ed3d8, 19, 687, 535}, // 120
        { 0x6e6b46639f77f0c8, 0x1c1962488c012ee8, 19, 622, 521},
        { 0xe2c6e5e725342f4a, 0x89955cc3a594e51a, 19, 578, 512},
        { 0x5c9a35ca819b38c8, 0xf9222e1acdf486e8, 19, 636, 492},
        { 0x4257e40e1c4713e3, 0xfe9696f9799c640b, 19, 553, 529},
        { 0x5b72bfac0ff3f885, 0x316e6b5e74bc7aa3, 20, 488, 480},
        { 0x4257e40e1c4713e3, 0xfe9696f9799c640b, 20, 483, 556},
        { 0x6e6b46639f77f0c8, 0x1c1962488c012ee8, 20, 465, 605},
        { 0xbc3d01fff751cde4, 0x6f948f09933cdfc, 20, 446, 649},
        { 0x60ef1d711ae30cf0, 0xc80756327d5de060, 20, 453, 561},
        { 0x4257e30e1c471230, 0xfe9695f9799c6258, 20, 320, 526}, // 130
        { 0xb8a5e528bfee71bc, 0xdf017f3fefba2704, 20, 442, 609},
        { 0x4c12cee7784c8b8, 0x7288f0346fd3cdd8, 20, 412, 566},
        { 0x50eaf4685fa07085, 0xf9280759d6cc62a3, 21, 947, 506},
        { 0xbc3d01fff751cde4, 0x6f948f09933cdfc, 22, 912, 467},
        { 0x5584521f1e549486, 0x55846e1f1e54c41a, 22, 925, 433},
        { 0xa178d4769765abac, 0xf4a830850f51d034, 22, 913, 408},
        { 0xc63dec8a65b5c540, 0x6aebee2a2d6d8470, 22, 895, 365},
        { 0x60ef1d711ae30cf0, 0xc80756327d5de060, 23, 526, 650},
        { 0x4257e40e1c4713e3, 0x4f1e5b1dd73edc57, 23, 576, 714},
        { 0x4c12cee7784c8b8, 0x7288f0346fd3cdd8, 23, 565, 726}, //140
        { 0x50eaf4685fa07085, 0xf9280759d6cc62a3, 23, 586, 726},
        { 0x4257e50e1c471596, 0xfe9697f9799c65be, 23, 621, 749},
        { 0x21f6c865b3513bab, 0xd8f0ffcde5822363, 23, 528, 695},
        { 0x5b72bfac0ff3f885, 0x316e6b5e74bc7aa3, 24, 408, 809},
        { 0xb0c9af2202b0a19e, 0x4f1e5c1dd73ede0a, 24, 426, 790},
        { 0x4257e30e1c471230, 0xfe9695f9799c6258, 24, 360, 850},
        { 0xa178d4769765abac, 0xf4a830850f51d034, 24, 327, 787},
        { 0x5c9a35ca819b38c8, 0xf9222e1acdf486e8, 25, 707, 421},
        { 0xdb8629cba3383296, 0x4f1e561dd73ed3d8, 25, 832, 398},
        { 0xb8a5e528bfee71bc, 0xdf017f3fefba2704, 25, 591, 430}, // 150
        { 0x79b25a4f80255a38, 0xc8ea8c1618ab0a58, 25, 666, 334},
        { 0x2998f2424d0353eb, 0xae57b2a84974c3a1, 25, 758, 338},
        { 0x6d015b7858eb5119, 0x4f1e5a1dd73edaa4, 25, 719, 377},
        { 0x21f6c865b3513bab, 0xd8f0ffcde5822363, 25, 659, 397},
        { 0x60ef1d711ae30cf0, 0x4f1e5d1dd73edfbd, 26, 665, 243},
        { 0x5b72bfac0ff3f885, 0x316e6b5e74bc7aa3, 26, 784, 212},
        { 0x79b25a4f80255a38, 0xc8ea8c1618ab0a58, 26, 881, 235},
        { 0x6b37a94863bf68c0, 0x4f1e591dd73ed8f1, 27, 321, 1004},
        { 0x4257ea0e1c471e15, 0xfe969cf9799c6e3d, 27, 782, 962},
        { 0x40bdbe4f3bcbac86, 0x9fdf11a0cde96b2e, 27, 1040, 752}, // 160
        { 0xc63dec8a65b5c540, 0x6aebee2a2d6d8470, 27, 970, 701},
        { 0x6d015b7858eb5119, 0x53441b80e563ef1f, 27, 759, 1015},
        { 0xa178d4769765abac, 0xf4a830850f51d034, 27, 558, 1082},
        { 0xb0c9af2202b0a19e, 0x3d6f1fcb3898d356, 27, 523, 993},
        { 0x60ef1d711ae30cf0, 0xc80756327d5de060, 28, 129, 797},
        { 0xb8a5e528bfee71bc, 0xdf017f3fefba2704, 28, 75, 658},
        { 0x6b37a94863bf68c0, 0x4f1e591dd73ed8f1, 28, 120, 523},
        { 0x5c9a35ca819b38c8, 0xf9222e1acdf486e8, 28, 120, 547},
        { 0x50eaf4685fa07085, 0xf9280759d6cc62a3, 28, 287, 559},
        { 0x21f6c965b3513d5e, 0xd8f100cde5822516, 28, 258, 654}, // 170
        { 0x4c12cee7784c8b8, 0x7288f0346fd3cdd8, 28, 174, 852},
        { 0x4257ea0e1c471e15, 0xfe969cf9799c6e3d, 28, 162, 808},
        { 0xa178d4769765abac, 0xf4a830850f51d034, 28, 162, 763},
        { 0xe2c6e5e725342f4a, 0x89955cc3a594e51a, 29, 299, 356},
        { 0x21f6c965b3513d5e, 0xd8f100cde5822516, 29, 214, 349},
        { 0x2998f2424d0353eb, 0xae57b2a84974c3a1, 29, 185, 302},
        { 0x4257ea0e1c471e15, 0xfe969cf9799c6e3d, 29, 247, 298},
        { 0x4257e30e1c471230, 0x4f1e581dd73ed73e, 29, 271, 273},
        { 0xb0c9af2202b0a19e, 0x3d6f1fcb3898d356, 30, 468, 451},
        { 0xbc3d01fff751cde4, 0x6f948f09933cdfc, 30, 605, 166}, // 180
        { 0xc63dec8a65b5c540, 0x6aebee2a2d6d8470, 30, 672, 120},
        { 0x4257ea0e1c471e15, 0xfe969cf9799c6e3d, 30, 716, 91},
        { 0x6e6b46639f77f0c8, 0x1c1962488c012ee8, 30, 597, 105},
        { 0xea4c3915ea6f95a0, 0x3ea9df3b7b2b5990, 31, 471, 152},
        { 0xea4c3915ea6f95a0, 0x3ea9df3b7b2b5990, 31, 490, 194},
        { 0xea4c3915ea6f95a0, 0x3ea9df3b7b2b5990, 31, 464, 237},
        { 0xea4c3915ea6f95a0, 0x3ea9df3b7b2b5990, 31, 413, 237},
        { 0xea4c3915ea6f95a0, 0x3ea9df3b7b2b5990, 31, 386, 195},
        { 0xea4c3915ea6f95a0, 0x3ea9df3b7b2b5990, 31, 414, 148}
    };
// clang-format on

namespace
{
    QHash<u64, Den> dens;
}

void DenLoader::init()
{
    // Normal and rare dens
    QFile f(":/encounters/nests.bin");
    if (f.open(QIODevice::ReadOnly))
    {
        QJsonObject doc(QJsonDocument::fromJson(qUncompress(f.readAll())).object());
        f.close();

        QJsonArray tables = doc["Tables"].toArray();
        for (auto i : tables)
        {
            QJsonObject table = i.toObject();
            QJsonArray swordEntries = table["SwordEntries"].toArray();
            QJsonArray shieldEntries = table["ShieldEntries"].toArray();

            QVector<Raid> swordRaids;
            for (auto j : swordEntries)
            {
                QJsonObject entry = j.toObject();

                u8 ability = static_cast<u8>(entry["Ability"].toInt());
                u8 altform = static_cast<u8>(entry["AltForm"].toInt());
                u8 ivCount = static_cast<u8>(entry["FlawlessIVs"].toInt());
                u8 gender = static_cast<u8>(entry["Gender"].toInt());
                bool gigantamax = entry["IsGigantamax"].toBool();
                u16 species = static_cast<u16>(entry["Species"].toInt());

                std::array<bool, 5> star;
                for (u8 k = 0; k < 5; k++)
                {
                    star[k] = entry["Stars"].toArray()[k].toBool();
                }

                swordRaids.append(Raid(ability, altform, ivCount, gender, gigantamax, species, star));
            }

            QVector<Raid> shieldRaids;
            for (auto j : shieldEntries)
            {
                QJsonObject entry = j.toObject();

                u8 ability = static_cast<u8>(entry["Ability"].toInt());
                u8 altform = static_cast<u8>(entry["AltForm"].toInt());
                u8 ivCount = static_cast<u8>(entry["FlawlessIVs"].toInt());
                u8 gender = static_cast<u8>(entry["Gender"].toInt());
                bool gigantamax = entry["IsGigantamax"].toBool();
                u16 species = static_cast<u16>(entry["Species"].toInt());

                std::array<bool, 5> star;
                for (u8 k = 0; k < 5; k++)
                {
                    star[k] = entry["Stars"].toArray()[k].toBool();
                }

                shieldRaids.append(Raid(ability, altform, ivCount, gender, gigantamax, species, star));
            }

            u64 hash = table["TableID"].toString().toULongLong(nullptr, 16);
            dens[hash] = Den(swordRaids, shieldRaids);
        }
    }

    // Event Dens
    f.setFileName(QApplication::applicationDirPath() + "/nests_event.json");
    if (f.open(QIODevice::ReadOnly))
    {
        QJsonObject doc(QJsonDocument::fromJson(f.readAll()).object());
        f.close();

        QJsonArray tables = doc["Tables"].toArray();
        QJsonArray swordEntries = tables.at(0).toObject()["Entries"].toArray();
        QJsonArray shieldEntries = tables.at(1).toObject()["Entries"].toArray();

        QVector<Raid> swordRaids;
        for (auto i : swordEntries)
        {
            QJsonObject entry = i.toObject();

            u8 ability = static_cast<u8>(entry["Ability"].toInt());
            u8 altform = static_cast<u8>(entry["AltForm"].toInt());
            u8 shinyType = static_cast<u8>(entry["ShinyForced"].toInt());
            u8 ivCount = static_cast<u8>(entry["FlawlessIVs"].toInt());
            u8 gender = static_cast<u8>(entry["Gender"].toInt());
            bool gigantamax = entry["IsGigantamax"].toBool();
            u16 species = static_cast<u16>(entry["Species"].toInt());

            std::array<bool, 5> star;
            for (u8 k = 0; k < 5; k++)
            {
                star[k] = entry["Probabilities"].toArray()[k].toInt() != 0;
            }

            if (std::any_of(std::begin(star), std::end(star), [](bool flag) { return flag; }))
            {
                swordRaids.append(Raid(ability, altform, ivCount, gender, gigantamax, species, star, shinyType));
            }
        }

        QVector<Raid> shieldRaids;
        for (auto i : shieldEntries)
        {
            QJsonObject entry = i.toObject();

            u8 ability = static_cast<u8>(entry["Ability"].toInt());
            u8 altform = static_cast<u8>(entry["AltForm"].toInt());
            u8 shinyType = static_cast<u8>(entry["ShinyForced"].toInt());
            u8 ivCount = static_cast<u8>(entry["FlawlessIVs"].toInt());
            u8 gender = static_cast<u8>(entry["Gender"].toInt());
            bool gigantamax = entry["IsGigantamax"].toBool();
            u16 species = static_cast<u16>(entry["Species"].toInt());

            std::array<bool, 5> star;
            for (u8 k = 0; k < 5; k++)
            {
                star[k] = entry["Probabilities"].toArray()[k].toInt() != 0;
            }

            if (std::any_of(std::begin(star), std::end(star), [](bool flag) { return flag; }))
            {
                shieldRaids.append(Raid(ability, altform, ivCount, gender, gigantamax, species, star, shinyType));
            }
        }
        dens[0x17e59bbd874fd95c] = Den(swordRaids, shieldRaids);
    }
}

Den DenLoader::getDen(u8 index, u8 rarity)
{
    u64 hash;
    if (index == 255)
    {
        hash = 0x17e59bbd874fd95c;
    }
    else
    {
        hash = denInfo[index][rarity];
    }

    return dens[hash];
}

u8 DenLoader::getLocation(u8 index)
{
    return static_cast<u8>(denInfo[index][2]);
}

QVector<u16> DenLoader::getCoordinates(u8 index)
{
    return { static_cast<u16>(denInfo[index][3]), static_cast<u16>(denInfo[index][4]) };
}
