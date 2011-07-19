/*

Copyright (c) 2009, 2010, 2011 STMicroelectronics
Written by Christophe Lyon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#ifdef __arm__
#include <arm_neon.h>
#else
#include "stm-arm-neon.h"
#endif

#include "stm-arm-neon-ref.h"

#ifndef INSN_NAME
#define INSN_NAME vmlal_lane
#define TEST_MSG "VMLAL_LANE"
#endif

#define FNNAME1(NAME) void exec_ ## NAME (void)
#define FNNAME(NAME) FNNAME1(NAME)

FNNAME (INSN_NAME)
{
  /* vector_res = vmlxl_lane(vector, vector3, vector4, lane),
     then store the result.  */
#define TEST_VMLXL_LANE1(INSN, T1, T2, W, W2, N, V)			\
  VECT_VAR(vector_res, T1, W, N) =                                      \
    INSN##_##T2##W2(VECT_VAR(vector, T1, W, N),                         \
                    VECT_VAR(vector3, T1, W2, N),                       \
                    VECT_VAR(vector4, T1, W2, N),                       \
                    V);                                                 \
  vst1q_##T2##W(VECT_VAR(result, T1, W, N), VECT_VAR(vector_res, T1, W, N))

#define TEST_VMLXL_LANE(INSN, T1, T2, W, W2, N, V)			\
  TEST_VMLXL_LANE1(INSN, T1, T2, W, W2, N, V)

  /* With ARM RVCT, we need to declare variables before any executable
     statement */
  DECL_VARIABLE(vector, int, 32, 4);
  DECL_VARIABLE(vector3, int, 16, 4);
  DECL_VARIABLE(vector4, int, 16, 4);
  DECL_VARIABLE(vector_res, int, 32, 4);

  DECL_VARIABLE(vector, int, 64, 2);
  DECL_VARIABLE(vector3, int, 32, 2);
  DECL_VARIABLE(vector4, int, 32, 2);
  DECL_VARIABLE(vector_res, int, 64, 2);

  DECL_VARIABLE(vector, uint, 32, 4);
  DECL_VARIABLE(vector3, uint, 16, 4);
  DECL_VARIABLE(vector4, uint, 16, 4);
  DECL_VARIABLE(vector_res, uint, 32, 4);

  DECL_VARIABLE(vector, uint, 64, 2);
  DECL_VARIABLE(vector3, uint, 32, 2);
  DECL_VARIABLE(vector4, uint, 32, 2);
  DECL_VARIABLE(vector_res, uint, 64, 2);

  clean_results ();

  TEST_VLOAD(vector, buffer, q, int, s, 32, 4);
  TEST_VLOAD(vector, buffer, q, int, s, 64, 2);
  TEST_VLOAD(vector, buffer, q, uint, u, 32, 4);
  TEST_VLOAD(vector, buffer, q, uint, u, 64, 2);

  TEST_VDUP(vector3, , int, s, 16, 4, 0x55);
  TEST_VDUP(vector4, , int, s, 16, 4, 0xBB);
  TEST_VDUP(vector3, , int, s, 32, 2, 0x55);
  TEST_VDUP(vector4, , int, s, 32, 2, 0xBB);
  TEST_VDUP(vector3, , uint, u, 16, 4, 0x55);
  TEST_VDUP(vector4, , uint, u, 16, 4, 0xBB);
  TEST_VDUP(vector3, , uint, u, 32, 2, 0x55);
  TEST_VDUP(vector4, , uint, u, 32, 2, 0xBB);

  TEST_VMLXL_LANE(INSN_NAME, int, s, 32, 16, 4, 2);
  TEST_VMLXL_LANE(INSN_NAME, int, s, 64, 32, 2, 1);
  TEST_VMLXL_LANE(INSN_NAME, uint, u, 32, 16, 4, 2);
  TEST_VMLXL_LANE(INSN_NAME, uint, u, 64, 32, 2, 1);

  dump_results_hex (TEST_MSG);
}
