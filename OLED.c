#include "OLED.h"

const unsigned char asc2_1608[][16] = {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /*" ",0*/
    {0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x30, 0x00, 0x00, 0x00}, /*"!",1*/
    {0x00, 0x10, 0x0C, 0x06, 0x10, 0x0C, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /*""",2*/
    {0x40, 0xC0, 0x78, 0x40, 0xC0, 0x78, 0x40, 0x00, 0x04, 0x3F, 0x04, 0x04, 0x3F, 0x04, 0x04, 0x00}, /*"#",3*/
    {0x00, 0x70, 0x88, 0xFC, 0x08, 0x30, 0x00, 0x00, 0x00, 0x18, 0x20, 0xFF, 0x21, 0x1E, 0x00, 0x00}, /*"$",4*/
    {0xF0, 0x08, 0xF0, 0x00, 0xE0, 0x18, 0x00, 0x00, 0x00, 0x21, 0x1C, 0x03, 0x1E, 0x21, 0x1E, 0x00}, /*"%",5*/
    {0x00, 0xF0, 0x08, 0x88, 0x70, 0x00, 0x00, 0x00, 0x1E, 0x21, 0x23, 0x24, 0x19, 0x27, 0x21, 0x10}, /*"&",6*/
    {0x10, 0x16, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /*"'",7*/
    {0x00, 0x00, 0x00, 0xE0, 0x18, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x07, 0x18, 0x20, 0x40, 0x00}, /*"(",8*/
    {0x00, 0x02, 0x04, 0x18, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x40, 0x20, 0x18, 0x07, 0x00, 0x00, 0x00}, /*")",9*/
    {0x40, 0x40, 0x80, 0xF0, 0x80, 0x40, 0x40, 0x00, 0x02, 0x02, 0x01, 0x0F, 0x01, 0x02, 0x02, 0x00}, /*"*",10*/
    {0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x1F, 0x01, 0x01, 0x01, 0x00}, /*"+",11*/
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xB0, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00}, /*",",12*/
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}, /*"-",13*/
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00}, /*".",14*/
    {0x00, 0x00, 0x00, 0x00, 0x80, 0x60, 0x18, 0x04, 0x00, 0x60, 0x18, 0x06, 0x01, 0x00, 0x00, 0x00}, /*"/",15*/
    {0x00, 0xE0, 0x10, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x00, 0x0F, 0x10, 0x20, 0x20, 0x10, 0x0F, 0x00}, /*"0",16*/
    {0x00, 0x10, 0x10, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00}, /*"1",17*/
    {0x00, 0x70, 0x08, 0x08, 0x08, 0x88, 0x70, 0x00, 0x00, 0x30, 0x28, 0x24, 0x22, 0x21, 0x30, 0x00}, /*"2",18*/
    {0x00, 0x30, 0x08, 0x88, 0x88, 0x48, 0x30, 0x00, 0x00, 0x18, 0x20, 0x20, 0x20, 0x11, 0x0E, 0x00}, /*"3",19*/
    {0x00, 0x00, 0xC0, 0x20, 0x10, 0xF8, 0x00, 0x00, 0x00, 0x07, 0x04, 0x24, 0x24, 0x3F, 0x24, 0x00}, /*"4",20*/
    {0x00, 0xF8, 0x08, 0x88, 0x88, 0x08, 0x08, 0x00, 0x00, 0x19, 0x21, 0x20, 0x20, 0x11, 0x0E, 0x00}, /*"5",21*/
    {0x00, 0xE0, 0x10, 0x88, 0x88, 0x18, 0x00, 0x00, 0x00, 0x0F, 0x11, 0x20, 0x20, 0x11, 0x0E, 0x00}, /*"6",22*/
    {0x00, 0x38, 0x08, 0x08, 0xC8, 0x38, 0x08, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00}, /*"7",23*/
    {0x00, 0x70, 0x88, 0x08, 0x08, 0x88, 0x70, 0x00, 0x00, 0x1C, 0x22, 0x21, 0x21, 0x22, 0x1C, 0x00}, /*"8",24*/
    {0x00, 0xE0, 0x10, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x00, 0x00, 0x31, 0x22, 0x22, 0x11, 0x0F, 0x00}, /*"9",25*/
    {0x00, 0x00, 0x00, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00}, /*":",26*/
    {0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x60, 0x00, 0x00, 0x00, 0x00}, /*";",27*/
    {0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x08, 0x00, 0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x00}, /*"<",28*/
    {0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00}, /*"=",29*/
    {0x00, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00}, /*">",30*/
    {0x00, 0x70, 0x48, 0x08, 0x08, 0x08, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x30, 0x36, 0x01, 0x00, 0x00}, /*"?",31*/
    {0xC0, 0x30, 0xC8, 0x28, 0xE8, 0x10, 0xE0, 0x00, 0x07, 0x18, 0x27, 0x24, 0x23, 0x14, 0x0B, 0x00}, /*"@",32*/
    {0x00, 0x00, 0xC0, 0x38, 0xE0, 0x00, 0x00, 0x00, 0x20, 0x3C, 0x23, 0x02, 0x02, 0x27, 0x38, 0x20}, /*"A",33*/
    {0x08, 0xF8, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00, 0x20, 0x3F, 0x20, 0x20, 0x20, 0x11, 0x0E, 0x00}, /*"B",34*/
    {0xC0, 0x30, 0x08, 0x08, 0x08, 0x08, 0x38, 0x00, 0x07, 0x18, 0x20, 0x20, 0x20, 0x10, 0x08, 0x00}, /*"C",35*/
    {0x08, 0xF8, 0x08, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x20, 0x3F, 0x20, 0x20, 0x20, 0x10, 0x0F, 0x00}, /*"D",36*/
    {0x08, 0xF8, 0x88, 0x88, 0xE8, 0x08, 0x10, 0x00, 0x20, 0x3F, 0x20, 0x20, 0x23, 0x20, 0x18, 0x00}, /*"E",37*/
    {0x08, 0xF8, 0x88, 0x88, 0xE8, 0x08, 0x10, 0x00, 0x20, 0x3F, 0x20, 0x00, 0x03, 0x00, 0x00, 0x00}, /*"F",38*/
    {0xC0, 0x30, 0x08, 0x08, 0x08, 0x38, 0x00, 0x00, 0x07, 0x18, 0x20, 0x20, 0x22, 0x1E, 0x02, 0x00}, /*"G",39*/
    {0x08, 0xF8, 0x08, 0x00, 0x00, 0x08, 0xF8, 0x08, 0x20, 0x3F, 0x21, 0x01, 0x01, 0x21, 0x3F, 0x20}, /*"H",40*/
    {0x00, 0x08, 0x08, 0xF8, 0x08, 0x08, 0x00, 0x00, 0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00}, /*"I",41*/
    {0x00, 0x00, 0x08, 0x08, 0xF8, 0x08, 0x08, 0x00, 0xC0, 0x80, 0x80, 0x80, 0x7F, 0x00, 0x00, 0x00}, /*"J",42*/
    {0x08, 0xF8, 0x88, 0xC0, 0x28, 0x18, 0x08, 0x00, 0x20, 0x3F, 0x20, 0x01, 0x26, 0x38, 0x20, 0x00}, /*"K",43*/
    {0x08, 0xF8, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x3F, 0x20, 0x20, 0x20, 0x20, 0x30, 0x00}, /*"L",44*/
    {0x08, 0xF8, 0xF8, 0x00, 0xF8, 0xF8, 0x08, 0x00, 0x20, 0x3F, 0x00, 0x3F, 0x00, 0x3F, 0x20, 0x00}, /*"M",45*/
    {0x08, 0xF8, 0x30, 0xC0, 0x00, 0x08, 0xF8, 0x08, 0x20, 0x3F, 0x20, 0x00, 0x07, 0x18, 0x3F, 0x00}, /*"N",46*/
    {0xE0, 0x10, 0x08, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x0F, 0x10, 0x20, 0x20, 0x20, 0x10, 0x0F, 0x00}, /*"O",47*/
    {0x08, 0xF8, 0x08, 0x08, 0x08, 0x08, 0xF0, 0x00, 0x20, 0x3F, 0x21, 0x01, 0x01, 0x01, 0x00, 0x00}, /*"P",48*/
    {0xE0, 0x10, 0x08, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x0F, 0x18, 0x24, 0x24, 0x38, 0x50, 0x4F, 0x00}, /*"Q",49*/
    {0x08, 0xF8, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00, 0x20, 0x3F, 0x20, 0x00, 0x03, 0x0C, 0x30, 0x20}, /*"R",50*/
    {0x00, 0x70, 0x88, 0x08, 0x08, 0x08, 0x38, 0x00, 0x00, 0x38, 0x20, 0x21, 0x21, 0x22, 0x1C, 0x00}, /*"S",51*/
    {0x18, 0x08, 0x08, 0xF8, 0x08, 0x08, 0x18, 0x00, 0x00, 0x00, 0x20, 0x3F, 0x20, 0x00, 0x00, 0x00}, /*"T",52*/
    {0x08, 0xF8, 0x08, 0x00, 0x00, 0x08, 0xF8, 0x08, 0x00, 0x1F, 0x20, 0x20, 0x20, 0x20, 0x1F, 0x00}, /*"U",53*/
    {0x08, 0x78, 0x88, 0x00, 0x00, 0xC8, 0x38, 0x08, 0x00, 0x00, 0x07, 0x38, 0x0E, 0x01, 0x00, 0x00}, /*"V",54*/
    {0xF8, 0x08, 0x00, 0xF8, 0x00, 0x08, 0xF8, 0x00, 0x03, 0x3C, 0x07, 0x00, 0x07, 0x3C, 0x03, 0x00}, /*"W",55*/
    {0x08, 0x18, 0x68, 0x80, 0x80, 0x68, 0x18, 0x08, 0x20, 0x30, 0x2C, 0x03, 0x03, 0x2C, 0x30, 0x20}, /*"X",56*/
    {0x08, 0x38, 0xC8, 0x00, 0xC8, 0x38, 0x08, 0x00, 0x00, 0x00, 0x20, 0x3F, 0x20, 0x00, 0x00, 0x00}, /*"Y",57*/
    {0x10, 0x08, 0x08, 0x08, 0xC8, 0x38, 0x08, 0x00, 0x20, 0x38, 0x26, 0x21, 0x20, 0x20, 0x18, 0x00}, /*"Z",58*/
    {0x00, 0x00, 0x00, 0xFE, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x40, 0x40, 0x40, 0x00}, /*"[",59*/
    {0x00, 0x0C, 0x30, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x06, 0x38, 0xC0, 0x00}, /*"\",60*/
    {0x00, 0x02, 0x02, 0x02, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x7F, 0x00, 0x00, 0x00}, /*"]",61*/
    {0x00, 0x00, 0x04, 0x02, 0x02, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /*"^",62*/
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80}, /*"_",63*/
    {0x00, 0x02, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /*"`",64*/
    {0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x19, 0x24, 0x22, 0x22, 0x22, 0x3F, 0x20}, /*"a",65*/
    {0x08, 0xF8, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x11, 0x20, 0x20, 0x11, 0x0E, 0x00}, /*"b",66*/
    {0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x0E, 0x11, 0x20, 0x20, 0x20, 0x11, 0x00}, /*"c",67*/
    {0x00, 0x00, 0x00, 0x80, 0x80, 0x88, 0xF8, 0x00, 0x00, 0x0E, 0x11, 0x20, 0x20, 0x10, 0x3F, 0x20}, /*"d",68*/
    {0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x1F, 0x22, 0x22, 0x22, 0x22, 0x13, 0x00}, /*"e",69*/
    {0x00, 0x80, 0x80, 0xF0, 0x88, 0x88, 0x88, 0x18, 0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00}, /*"f",70*/
    {0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x6B, 0x94, 0x94, 0x94, 0x93, 0x60, 0x00}, /*"g",71*/
    {0x08, 0xF8, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x20, 0x3F, 0x21, 0x00, 0x00, 0x20, 0x3F, 0x20}, /*"h",72*/
    {0x00, 0x80, 0x98, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00}, /*"i",73*/
    {0x00, 0x00, 0x00, 0x80, 0x98, 0x98, 0x00, 0x00, 0x00, 0xC0, 0x80, 0x80, 0x80, 0x7F, 0x00, 0x00}, /*"j",74*/
    {0x08, 0xF8, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x20, 0x3F, 0x24, 0x02, 0x2D, 0x30, 0x20, 0x00}, /*"k",75*/
    {0x00, 0x08, 0x08, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00}, /*"l",76*/
    {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x20, 0x3F, 0x20, 0x00, 0x3F, 0x20, 0x00, 0x3F}, /*"m",77*/
    {0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x20, 0x3F, 0x21, 0x00, 0x00, 0x20, 0x3F, 0x20}, /*"n",78*/
    {0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x1F, 0x20, 0x20, 0x20, 0x20, 0x1F, 0x00}, /*"o",79*/
    {0x80, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x80, 0xFF, 0xA1, 0x20, 0x20, 0x11, 0x0E, 0x00}, /*"p",80*/
    {0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x0E, 0x11, 0x20, 0x20, 0xA0, 0xFF, 0x80}, /*"q",81*/
    {0x80, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x00, 0x20, 0x20, 0x3F, 0x21, 0x20, 0x00, 0x01, 0x00}, /*"r",82*/
    {0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x33, 0x24, 0x24, 0x24, 0x24, 0x19, 0x00}, /*"s",83*/
    {0x00, 0x80, 0x80, 0xE0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x20, 0x20, 0x00, 0x00}, /*"t",84*/
    {0x80, 0x80, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x1F, 0x20, 0x20, 0x20, 0x10, 0x3F, 0x20}, /*"u",85*/
    {0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x01, 0x0E, 0x30, 0x08, 0x06, 0x01, 0x00}, /*"v",86*/
    {0x80, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x80, 0x0F, 0x30, 0x0C, 0x03, 0x0C, 0x30, 0x0F, 0x00}, /*"w",87*/
    {0x00, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x20, 0x31, 0x2E, 0x0E, 0x31, 0x20, 0x00}, /*"x",88*/
    {0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x81, 0x8E, 0x70, 0x18, 0x06, 0x01, 0x00}, /*"y",89*/
    {0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x21, 0x30, 0x2C, 0x22, 0x21, 0x30, 0x00}, /*"z",90*/
    {0x00, 0x00, 0x00, 0x00, 0x80, 0x7C, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x40, 0x40}, /*"{",91*/
    {0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00}, /*"|",92*/
    {0x00, 0x02, 0x02, 0x7C, 0x80, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x3F, 0x00, 0x00, 0x00, 0x00}, /*"}",93*/
    {0x00, 0x06, 0x01, 0x01, 0x02, 0x02, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /*"~",94*/
};

u8 OLED_GRAM[144][4];

// 反显函数
void OLED_ColorTurn(u8 i) {
  if (i == 0) {
    OLED_WR_Byte(0xA6, OLED_CMD);  // 正常显示
  }
  if (i == 1) {
    OLED_WR_Byte(0xA7, OLED_CMD);  // 反色显示
  }
}

// 屏幕旋转180度
void OLED_DisplayTurn(u8 i) {
  if (i == 0) {
    OLED_WR_Byte(0xC8, OLED_CMD);  // 正常显示
    OLED_WR_Byte(0xA1, OLED_CMD);
  }
  if (i == 1) {
    OLED_WR_Byte(0xC0, OLED_CMD);  // 反转显示
    OLED_WR_Byte(0xA0, OLED_CMD);
  }
}

// 延时
void IIC_delay(void) {
  u8 t = 10;
  while (t--)
    ;
}

// 起始信号
void I2C_Start(void) {
  OLED_SDA_Set();
  OLED_SCL_Set();
  IIC_delay();
  OLED_SDA_Clr();
  IIC_delay();
  OLED_SCL_Clr();
  IIC_delay();
}

// 结束信号
void I2C_Stop(void) {
  OLED_SDA_Clr();
  OLED_SCL_Set();
  IIC_delay();
  OLED_SDA_Set();
}

// 等待信号响应
void I2C_WaitAck(void)  // 测数据信号的电平
{
  OLED_SDA_Set();
  IIC_delay();
  OLED_SCL_Set();
  IIC_delay();
  OLED_SCL_Clr();
  IIC_delay();
}

// 写入一个字节
void Send_Byte(u8 dat) {
  u8 i;
  for (i = 0; i < 8; i++) {
    if (dat & 0x80)  // 将dat的8位从最高位依次写入
    {
      OLED_SDA_Set();
    } else {
      OLED_SDA_Clr();
    }
    IIC_delay();
    OLED_SCL_Set();
    IIC_delay();
    OLED_SCL_Clr();  // 将时钟信号设置为低电平
    dat <<= 1;
  }
}

// 发送一个字节
// mode:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat, u8 mode) {
  I2C_Start();
  Send_Byte(0x78);
  I2C_WaitAck();
  if (mode) {
    Send_Byte(0x40);
  } else {
    Send_Byte(0x00);
  }
  I2C_WaitAck();
  Send_Byte(dat);
  I2C_WaitAck();
  I2C_Stop();
}

// 开启OLED显示
void OLED_DisPlay_On(void) {
  OLED_WR_Byte(0x8D, OLED_CMD);  // 电荷泵使能
  OLED_WR_Byte(0x14, OLED_CMD);  // 开启电荷泵
  OLED_WR_Byte(0xAF, OLED_CMD);  // 点亮屏幕
}

// 关闭OLED显示
void OLED_DisPlay_Off(void) {
  OLED_WR_Byte(0x8D, OLED_CMD);  // 电荷泵使能
  OLED_WR_Byte(0x10, OLED_CMD);  // 关闭电荷泵
  OLED_WR_Byte(0xAE, OLED_CMD);  // 关闭屏幕
}

// 更新显存到OLED
void OLED_Refresh(void) {
  u8 i, n;
  for (i = 0; i < 4; i++) {
    OLED_WR_Byte(0xb0 + i, OLED_CMD);  // 设置行起始地址
    OLED_WR_Byte(0x00, OLED_CMD);      // 设置低列起始地址
    OLED_WR_Byte(0x10, OLED_CMD);      // 设置高列起始地址
    I2C_Start();
    Send_Byte(0x78);
    I2C_WaitAck();
    Send_Byte(0x40);
    I2C_WaitAck();
    for (n = 0; n < 128; n++) {
      Send_Byte(OLED_GRAM[n][i]);
      I2C_WaitAck();
    }
    I2C_Stop();
  }
}
// 清屏函数
void OLED_Clear(void) {
  u8 i, n;
  for (i = 0; i < 4; i++) {
    for (n = 0; n < 128; n++) {
      OLED_GRAM[n][i] = 0;  // 清除所有数据
    }
  }
  OLED_Refresh();  // 更新显示
}

// 画点
// x:0~127
// y:0~63
// t:1 填充 0,清空
void OLED_DrawPoint(u8 x, u8 y, u8 t) {
  u8 i, m, n;
  i = y / 8;
  m = y % 8;
  n = 1 << m;
  if (t) {
    OLED_GRAM[x][i] |= n;
  } else {
    OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
    OLED_GRAM[x][i] |= n;
    OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
  }
}

// 画线
// x1,y1:起点坐标
// x2,y2:结束坐标
void OLED_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2, u8 mode) {
  u16 t;
  int xerr = 0, yerr = 0, delta_x, delta_y, distance;
  int incx, incy, uRow, uCol;
  delta_x = x2 - x1;  // 计算坐标增量
  delta_y = y2 - y1;
  uRow = x1;  // 画线起点坐标
  uCol = y1;
  if (delta_x > 0)
    incx = 1;  // 设置单步方向
  else if (delta_x == 0)
    incx = 0;  // 垂直线
  else {
    incx = -1;
    delta_x = -delta_x;
  }
  if (delta_y > 0)
    incy = 1;
  else if (delta_y == 0)
    incy = 0;  // 水平线
  else {
    incy = -1;
    delta_y = -delta_x;
  }
  if (delta_x > delta_y)
    distance = delta_x;  // 选取基本增量坐标轴
  else
    distance = delta_y;
  for (t = 0; t < distance + 1; t++) {
    OLED_DrawPoint(uRow, uCol, mode);  // 画点
    xerr += delta_x;
    yerr += delta_y;
    if (xerr > distance) {
      xerr -= distance;
      uRow += incx;
    }
    if (yerr > distance) {
      yerr -= distance;
      uCol += incy;
    }
  }
}
// x,y:圆心坐标
// r:圆的半径
void OLED_DrawCircle(u8 x, u8 y, u8 r) {
  int a, b, num;
  a = 0;
  b = r;
  while (2 * b * b >= r * r) {
    OLED_DrawPoint(x + a, y - b, 1);
    OLED_DrawPoint(x - a, y - b, 1);
    OLED_DrawPoint(x - a, y + b, 1);
    OLED_DrawPoint(x + a, y + b, 1);

    OLED_DrawPoint(x + b, y + a, 1);
    OLED_DrawPoint(x + b, y - a, 1);
    OLED_DrawPoint(x - b, y - a, 1);
    OLED_DrawPoint(x - b, y + a, 1);

    a++;
    num = (a * a + b * b) - r * r;  // 计算画的点离圆心的距离
    if (num > 0) {
      b--;
      a--;
    }
  }
}

// 在指定位置显示一个字符,包括部分字符
// x:0~127
// y:0~63
// size1:选择字体 6x8/6x12/8x16/12x24
// mode:0,反色显示;1,正常显示
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 size1, u8 mode) {
  u8 i, m, temp, size2, chr1;
  u8 x0 = x, y0 = y;
  if (size1 == 8)
    size2 = 6;
  else
    size2 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * (size1 / 2);  // 得到字体一个字符对应点阵集所占的字节数
  chr1 = chr - ' ';                                             // 计算偏移后的值
  for (i = 0; i < size2; i++) {
    if (size1 == 16) {
      temp = asc2_1608[chr1][i];
    }  // 调用1608字体
    else
      return;
    for (m = 0; m < 8; m++) {
      if (temp & 0x01)
        OLED_DrawPoint(x, y, mode);
      else
        OLED_DrawPoint(x, y, !mode);
      temp >>= 1;
      y++;
    }
    x++;
    if ((size1 != 8) && ((x - x0) == size1 / 2)) {
      x = x0;
      y0 = y0 + 8;
    }
    y = y0;
  }
}

// 显示字符串
// x,y:起点坐标
// size1:字体大小
//*chr:字符串起始地址
// mode:0,反色显示;1,正常显示
void OLED_ShowString(u8 x, u8 y, u8 *chr, u8 size1, u8 mode) {
  while ((*chr >= ' ') && (*chr <= '~'))  // 判断是不是非法字符!
  {
    OLED_ShowChar(x, y, *chr, size1, mode);
    if (size1 == 8)
      x += 6;
    else
      x += size1 / 2;
    chr++;
  }
}

// m^n
u32 OLED_Pow(u8 m, u8 n) {
  u32 result = 1;
  while (n--) {
    result *= m;
  }
  return result;
}

// 显示数字
// x,y :起点坐标
// len :数字的位数
// size:字体大小
// mode:0,反色显示;1,正常显示
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size1, u8 mode) {
  u8 t, temp, m = 0;
  if (size1 == 8) m = 2;
  for (t = 0; t < len; t++) {
    temp = (num / OLED_Pow(10, len - t - 1)) % 10;
    if (temp == 0) {
      OLED_ShowChar(x + (size1 / 2 + m) * t, y, '0', size1, mode);
    } else {
      OLED_ShowChar(x + (size1 / 2 + m) * t, y, temp + '0', size1, mode);
    }
  }
}

// x,y：起点坐标
// sizex,sizey,图片长宽
// BMP[]：要写入的图片数组
// mode:0,反色显示;1,正常显示
void OLED_ShowPicture(u8 x, u8 y, u8 sizex, u8 sizey, u8 BMP[], u8 mode) {
  u16 j = 0;
  u8 i, n, temp, m;
  u8 x0 = x, y0 = y;
  sizey = sizey / 8 + ((sizey % 8) ? 1 : 0);
  for (n = 0; n < sizey; n++) {
    for (i = 0; i < sizex; i++) {
      temp = BMP[j];
      j++;
      for (m = 0; m < 8; m++) {
        if (temp & 0x01)
          OLED_DrawPoint(x, y, mode);
        else
          OLED_DrawPoint(x, y, !mode);
        temp >>= 1;
        y++;
      }
      x++;
      if ((x - x0) == sizex) {
        x = x0;
        y0 = y0 + 8;
      }
      y = y0;
    }
  }
}
// OLED的初始化
void OLED_Init(void) {
  EALLOW;
  GpioCtrlRegs.GPBPUD.bit.GPIO41 = 0;    // Enable pullup on GPIO41
  GpioCtrlRegs.GPBMUX1.bit.GPIO41 = 0;   // GPIO41 = GPIO41
  GpioCtrlRegs.GPBDIR.bit.GPIO41 = 1;    // GPIO41 = output
  GpioDataRegs.GPBCLEAR.bit.GPIO41 = 1;  // Load output latch

  GpioCtrlRegs.GPBPUD.bit.GPIO40 = 0;    // Enable pullup on GPIO40
  GpioCtrlRegs.GPBMUX1.bit.GPIO40 = 0;   // GPIO40 = GPIO40
  GpioCtrlRegs.GPBDIR.bit.GPIO40 = 1;    // GPIO40 = output
  GpioDataRegs.GPBCLEAR.bit.GPIO40 = 1;  // Load output latch
  EDIS;

  OLED_RES_Clr();
  DELAY_US(200000);
  OLED_RES_Set();

  OLED_WR_Byte(0xAE, OLED_CMD); /*display off*/
  OLED_WR_Byte(0x00, OLED_CMD); /*set lower column address*/
  OLED_WR_Byte(0x10, OLED_CMD); /*set higher column address*/
  OLED_WR_Byte(0x00, OLED_CMD); /*set display start line*/
  OLED_WR_Byte(0xB0, OLED_CMD); /*set page address*/
  OLED_WR_Byte(0x81, OLED_CMD); /*contract control*/
  OLED_WR_Byte(0xff, OLED_CMD); /*128*/
  OLED_WR_Byte(0xA1, OLED_CMD); /*set segment remap*/
  OLED_WR_Byte(0xA6, OLED_CMD); /*normal / reverse*/
  OLED_WR_Byte(0xA8, OLED_CMD); /*multiplex ratio*/
  OLED_WR_Byte(0x1F, OLED_CMD); /*duty = 1/32*/
  OLED_WR_Byte(0xC8, OLED_CMD); /*Com scan direction*/
  OLED_WR_Byte(0xD3, OLED_CMD); /*set display offset*/
  OLED_WR_Byte(0x00, OLED_CMD);
  OLED_WR_Byte(0xD5, OLED_CMD); /*set osc division*/
  OLED_WR_Byte(0x80, OLED_CMD);
  OLED_WR_Byte(0xD9, OLED_CMD); /*set pre-charge period*/
  OLED_WR_Byte(0x1f, OLED_CMD);
  OLED_WR_Byte(0xDA, OLED_CMD); /*set COM pins*/
  OLED_WR_Byte(0x00, OLED_CMD);
  OLED_WR_Byte(0xdb, OLED_CMD); /*set vcomh*/
  OLED_WR_Byte(0x40, OLED_CMD);
  OLED_WR_Byte(0x8d, OLED_CMD); /*set charge pump enable*/
  OLED_WR_Byte(0x14, OLED_CMD);
  OLED_Clear();
  OLED_WR_Byte(0xAF, OLED_CMD); /*display ON*/
}
