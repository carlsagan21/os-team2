/*
 *
 * (C) COPYRIGHT 2014-2015 ARM Limited. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 *
 * A copy of the licence is included with the program, and can also be obtained
 * from Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 *
 */




/*
 * Register-based HW access backend specific definitions
 */

#ifndef _KBASE_HWACCESS_GPU_DEFS_H_
#define _KBASE_HWACCESS_GPU_DEFS_H_

/* SLOT_RB_SIZE must be < 256 */
#define SLOT_RB_SIZE 2
#define SLOT_RB_MASK (SLOT_RB_SIZE - 1)

/**
 * struct rb_entry - Ringbuffer entry
 * @katom:	Atom associated with this entry
 */
struct rb_entry {
	struct kbase_jd_atom *katom;
};

/**
 * struct slot_rb - Slot ringbuffer
 * @entries:		Ringbuffer entries
 * @last_context:	The last context to submit a job on this slot
 * @read_idx:		Current read index of buffer
 * @write_idx:		Current write index of buffer
 * @job_chain_flag:	Flag used to implement jobchain disambiguation
 */
struct slot_rb {
	struct rb_entry entries[SLOT_RB_SIZE];

	struct kbase_context *last_context;

	u8 read_idx;
	u8 write_idx;

	u8 job_chain_flag;
};

/**
 * struct kbase_backend_data - GPU backend specific data for HW access layer
 * @slot_rb:			Slot ringbuffers
 * @rmu_workaround_flag:	When PRLAM-8987 is present, this flag determines
 *				whether slots 0/1 or slot 2 are currently being
 *				pulled from
 * @scheduling_timer:		The timer tick used for rescheduling jobs
 * @timer_running:		Is the timer running? The runpool_mutex must be
 *				held whilst modifying this.
 *
 * The kbasep_js_device_data::runpool_irq::lock (a spinlock) must be held when
 * accessing this structure
 */
struct kbase_backend_data {
	struct slot_rb slot_rb[BASE_JM_MAX_NR_SLOTS];

	bool rmu_workaround_flag;

	struct hrtimer scheduling_timer;

	bool timer_running;

	/* Set when we're about to reset the GPU */
	atomic_t reset_gpu;

/* The GPU reset isn't pending */
#define KBASE_RESET_GPU_NOT_PENDING     0
/* kbase_prepare_to_reset_gpu has been called */
#define KBASE_RESET_GPU_PREPARED        1
/* kbase_reset_gpu has been called - the reset will now definitely happen
 * within the timeout period */
#define KBASE_RESET_GPU_COMMITTED       2
/* The GPU reset process is currently occuring (timeout has expired or
 * kbasep_try_reset_gpu_early was called) */
#define KBASE_RESET_GPU_HAPPENING       3

	/* Work queue and work item for performing the reset in */
	struct workqueue_struct *reset_workq;
	struct work_struct reset_work;
	wait_queue_head_t reset_wait;
	struct hrtimer reset_timer;
};

/**
 * struct kbase_jd_atom_backend - GPU backend specific katom data
 */
struct kbase_jd_atom_backend {
};

/**
 * struct kbase_context_backend - GPU backend specific context data
 */
struct kbase_context_backend {
};

#endif /* _KBASE_HWACCESS_GPU_DEFS_H_ */
