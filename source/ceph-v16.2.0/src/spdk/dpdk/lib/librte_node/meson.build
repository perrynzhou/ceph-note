# SPDX-License-Identifier: BSD-3-Clause
# Copyright(C) 2020 Marvell International Ltd.

sources = files('null.c', 'log.c', 'ethdev_rx.c', 'ethdev_tx.c', 'ip4_lookup.c',
		'ip4_rewrite.c', 'pkt_drop.c', 'ethdev_ctrl.c')
headers = files('rte_node_ip4_api.h', 'rte_node_eth_api.h')
# Strict-aliasing rules are violated by uint8_t[] to context size casts.
cflags += '-fno-strict-aliasing'
deps += ['graph', 'mbuf', 'lpm', 'ethdev', 'mempool', 'cryptodev']
build = false
reason = 'not needed by SPDK'
