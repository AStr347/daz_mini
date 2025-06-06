/**
 * Generic file for all used net instances and functions
 * contain functions, to work with various networks
 */
#ifndef _NETLAYER_GENERIC_H
#define _NETLAYER_GENERIC_H
#include "netlayer_generic_iface.h"

#define net_off(__NET__)                                        (__NET__->nft->off(__NET__->this))
#define net_on(__NET__)                                         (__NET__->nft->on(__NET__->this))
#define net_normal(__NET__)                                     (__NET__->nft->normal(__NET__->this))
#define net_test_start(__NET__)                                 (__NET__->nft->test_start(__NET__->this))

#define net_pared_del(__NET__, __ID__)                          (__NET__->nft->pared_del(__NET__->this, __ID__))
#define net_pared_put(__NET__, __ID__)                          (__NET__->nft->pared_put(__NET__->this, __ID__))
#define net_paring_on(__NET__, __CNT__)                         (__NET__->nft->paring_on(__NET__->this, __CNT__))
#define net_disparing_on(__NET__, __CNT__)                      (__NET__->nft->disparing_on(__NET__->this, __CNT__))

#define net_read(__NET__, __MESSAGE__)                          (__NET__->nft->read(__NET__->this, __MESSAGE__))
#define net_info_tx(__NET__, __ID__, __DATA__)                  (__NET__->nft->info_tx(__NET__->this, __ID__, __DATA__))
#define net_flush(__NET__)                                      (__NET__->nft->flush(__NET__->this))

#define net_mac_req_on(__NET__)                                 (__NET__->nft->mac_req_on(__NET__->this))
#define net_mac_gen_on(__NET__, __START__)                      (__NET__->nft->mac_gen_on(__NET__->this, __START__))

#define net_mac_set(__NET__, __ID__)                            (__NET__->nft->mac_set(__NET__->this, __ID__))
#define net_beh_set(__NET__, __BEH__)                           (__NET__->nft->beh_set(__NET__->this, __BEH__))

#define net_cancel_tx(__NET__, __ID__, __DATA__)                (__NET__->nft->cancel_tx(__NET__->this, __ID__, __DATA__))
#define net_cancel_bcast(__NET__, __DATA__)                     (__NET__->nft->cancel_bcast(__NET__->this, __DATA__))
#define net_info_bcast(__NET__, __DATA__)                       (__NET__->nft->info_bcast(__NET__->this, __DATA__))

/* easy access to net0 */
#define net0_off()                                      (net0->nft->off(net0->this))
#define net0_on()                                       (net0->nft->on(net0->this))
#define net0_normal()                                   (net0->nft->normal(net0->this))
#define net0_test_start()                               (net0->nft->test_start(net0->this))

#define net0_paring_on(__CNT__)                         (net0->nft->paring_on(net0->this, __CNT__))
#define net0_disparing_on(__CNT__)                      (net0->nft->disparing_on(net0->this, __CNT__))

#define net0_read(__MESSAGE__)                          (net0->nft->read(net0->this, __MESSAGE__))
#define net0_info_tx(__ID__, __DATA__)                  (net0->nft->info_tx(net0->this, __ID__, __DATA__))
#define net0_flush()                                    (net0->nft->flush(net0->this))

#define net0_mac_req_on()                               (net0->nft->mac_req_on(net0->this))
#define net0_mac_gen_on(__START__)                      (net0->nft->mac_gen_on(net0->this, __START__))

#define net0_mac_set(__ID__)                            (net0->nft->mac_set(net0->this, __ID__))
#define net0_beh_set(__BEH__)                           (net0->nft->beh_set(net0->this, __BEH__))

#define net0_cancel_tx(__ID__, __DATA__)                (net0->nft->cancel_tx(net0->this, __ID__, __DATA__))
#define net0_cancel_bcast(__DATA__)                     (net0->nft->cancel_bcast(net0->this, __DATA__))
#define net0_info_bcast(__DATA__)                       (net0->nft->info_bcast(net0->this, __DATA__))

/* easy access to net1 */
#define net1_off()                                      (net1->nft->off(net1->this))
#define net1_on()                                       (net1->nft->on(net1->this))
#define net1_normal()                                   (net1->nft->normal(net1->this))
#define net1_test_start()                               (net1->nft->test_start(net1->this))

#define net1_paring_on(__CNT__)                         (net1->nft->paring_on(net1->this, __CNT__))
#define net1_disparing_on(__CNT__)                      (net1->nft->disparing_on(net1->this, __CNT__))

#define net1_read(__MESSAGE__)                          (net1->nft->read(net1->this, __MESSAGE__))
#define net1_info_tx(__ID__, __DATA__)                  (net1->nft->info_tx(net1->this, __ID__, __DATA__))
#define net1_flush()                                    (net1->nft->flush(net1->this))

#define net1_mac_req_on()                               (net1->nft->mac_req_on(net1->this))
#define net1_mac_gen_on(__START__)                      (net1->nft->mac_gen_on(net1->this, __START__))

#define net1_mac_set(__ID__)                            (net1->nft->mac_set(net1->this, __ID__))
#define net1_beh_set(__BEH__)                           (net1->nft->beh_set(net1->this, __BEH__))

#define net1_cancel_tx(__ID__, __DATA__)                (net1->nft->cancel_tx(net1->this, __ID__, __DATA__))
#define net1_cancel_bcast(__DATA__)                     (net1->nft->cancel_bcast(net1->this, __DATA__))
#define net1_info_bcast(__DATA__)                       (net1->nft->info_bcast(net1->this, __DATA__))

#endif /* _NETLAYER_GENERIC_H */
