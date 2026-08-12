#ifndef __MY_RING_H
#define __MY_RING_H


struct ring_buf_base_t {
    const char* name;
};
struct ring_buf_base_t* g_ut2_rx_ring_pst;
struct ring_buf_base_t* g_ut2_tx_ring_pst;


#endif
