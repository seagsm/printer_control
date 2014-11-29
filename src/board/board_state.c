
#include "board_state.h"




static BOARD_SYSTEM_STATE   bss_current_state;
static BOARD_SYSTEM_STATE   bss_required_state;

/* This function to read current board state. */
BOARD_SYSTEM_STATE bss_board_stateGetCurrentState(void)
{
    return(bss_current_state);
}
/* This function to write current board state. */
void v_board_stateUpdateCurrentState(BOARD_SYSTEM_STATE   bss_set_state)
{
    bss_current_state = bss_set_state;
}

/* This function to read required board state. */
BOARD_SYSTEM_STATE bss_board_stateGetRequiredState(void)
{
    return(bss_required_state);
}
/* This function to require new board state. */
void v_board_stateSetRequiredState(BOARD_SYSTEM_STATE   bss_set_state)
{
    bss_required_state = bss_set_state;
}