/*
 * msdl.h
 *
 *  Created on: 20 feb. 2024
 *      Author: Guillaume Fouilleul
 */

#ifndef INC_MSDL_H_
#define INC_MSDL_H_

#include <stdbool.h>

#define MSDL_SCALE 5

int msdl_init(void);
void msdl_deinit(void);
bool msdl_loop(bool Render);

#ifdef PPU_DEBUG
void msdl_render_debug(void);
#endif

#endif /* INC_MSDL_H_ */
