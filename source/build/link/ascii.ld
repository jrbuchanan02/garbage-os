/*
 * File: ascii.ld
 * Author: Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * Brief: Defines ASCII characters as different values for use in a linker script
 * Version: 1
 * Date: 2023-04-16
 * Copyright: This file is part of Garbage OS. Garbage OS is Copyright (C) 2023
 * Joshua Buchanan and published under the LGPL license version 2.1. You are 
 * free to use this source code in your project even if your code is not open 
 * source. ASCII is a published standard by ANSII (a now shut-down department of the
 * US Government's executive branch). ASCII is also a subset of UTF-8 and so
 * these characters are also valid UTF-8 characters.
 */

nul = 0x00;
space = 0x20;
ex_pt = 0x21;
quote = 0x22;
pound = 0x23;
dollar = 0x24;
percnt = 0x25;
amp = 0x26;
apostrophe = 0x27;
paren_open = 0x28;
paren_close = 0x29;
asterisk = 0x2A;
plus = 0x2B;
comma = 0x2C;
dash = 0x2D;
period = 0x2E;
slash_f = 0x2F;
zero = 0x30;
one = 0x31;
two = 0x32;
three = 0x33;
four = 0x34;
five = 0x35;
six = 0x36;
seven = 0x37;
eight = 0x38;
nine = 0x39;
colon = 0x3A;
semicolon = 0x3B;
less_than = 0x3C;
equals = 0x3D;
greater_than = 0x3E;
q_mark = 0x3F;
at_sign = 0x40;
A = 0x41;
B = 0x42;
C = 0x43;
D = 0x44;
E = 0x45;
F = 0x46;
G = 0x47;
H = 0x48;
I = 0x49;
J = 0x4A;
K = 0x4B;
L = 0x4C;
M = 0x4D;
N = 0x4E;
O = 0x4F;
P = 0x50;
Q = 0x51;
R = 0x52;
S = 0x53;
T = 0x54;
U = 0x55;
V = 0x56;
W = 0x57;
X = 0x58;
Y = 0x59;
Z = 0x5A;
bracket_open = 0x5B;
backslash = 0x5C;
bracket_close = 0x5D;
carat = 0x5E;
u_score = 0x5F;
backtick = 0x60;
letter_a = 0x61;
letter_b = 0x62;
letter_c = 0x63;
letter_d = 0x64;
letter_e = 0x65;
letter_f = 0x66;
letter_g = 0x67;
letter_h = 0x68;
letter_i = 0x69;
letter_j = 0x6A;
letter_k = 0x6B;
letter_l = 0x6C;
letter_m = 0x6D;
letter_n = 0x6E;
letter_o = 0x6F;
letter_p = 0x70;
letter_q = 0x71;
letter_r = 0x72;
letter_s = 0x73;
letter_t = 0x74;
letter_u = 0x75;
letter_v = 0x76;
letter_w = 0x77;
letter_x = 0x78;
letter_y = 0x79;
letter_z = 0x7A;
curly_open = 0x7B;
pipe = 0x7C;
curly_close = 0x7D;
tilde = 0x7E;
delete = 0x7F;