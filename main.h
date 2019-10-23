#ifndef MAIN_H
#define MAIN_H

#ifdef _LANGUAGE_C

extern NUContData contdata[1]; /* Read data of the controller  */
extern u8 contPattern;         /* The pattern of the connected controller  */

#endif /* _LANGUAGE_C */
#endif /* MAIN_H */

void stage00(int);
void initStage00(void);
void makeDL00(void);
void updateGame00(void);