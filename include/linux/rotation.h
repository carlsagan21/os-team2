#include <linux/types.h>
#include <linux/list.h>
#include <linux/wait.h>

/**
 * type. 락의 종류가 리드인지 라이트인지를 정의합니다.
 */
#define READ_LOCK 0
#define WRITE_LOCK 1

/**
 * 리스트를 4개로 구현하였습니다.
 * PENDING 리스트에는 현재의 rotation 값을 포함하지 않아서, acquire될 가능성이 없는 락들을 저장합니다.
 * WAIT_READ 에는 리드 락이면서 로테이션값에 포함되어, 락을 잡을 가능성이 있는 락들이 저장됩니다.
 * WAIT_WRITE는 WAIT_READ와 유사합니다.
 * ACQUIRED 에는 잡은 락들이 저장됩니다.
 * 자료구로를 이렇게 4개로 나눈 까닭은, wait starvation 방지를 보다 간명하게 하기 위함입니다.
 * 이 자료구조 하에서는, WAIT_WRITE에 락이 적어도 하나 있는지를 판단하는 것만으로 스타베이션 상황을 막을 수 있습니다.
 */
#define PENDING 0
#define WAIT_READ 1
#define WAIT_WRITE 2
#define ACQUIRED 3

/**
 * 리스트에 따른 리스트 헤드를 extern 으로 선언합니다.
 */
extern struct list_head pending_lh;
extern struct list_head wait_read_lh;
extern struct list_head wait_write_lh;
extern struct list_head acquired_lh;

/**
 * set_rotation 에 의해 저장되는 로테이션 값을 저장하는 구조입니다.
 * 지금와서 생각하면 struct 일 필요는 없었던 것 같습니다. 초기에 이 안에 추가적인 정보를 넣으려고 기획했다보니 스트럭트로 선언하게 되었습니다.
 */
typedef struct __rotation_t {
	int degree;
} rotation_t;

extern rotation_t rotation;

/**
 * 시스템 콜의 critical area 의 atomicity 를 보장하기 위한 뮤텍스입니다.
 * 시스템 콜 내부에서 리스트를 다루는 여러 함수(list_add_pending, refresh_pending_waiting_lists, wait_write_to_acquire, wait_read_to_acquire)를 순차적으로 호출하게 되므로,
 * 이것들을 익스클루시브 하게 만들어줄 필요가 있습니다.
 */
extern struct mutex rotlock_mutex;

/**
 * 실제적으로 쓰레드를 를 재우고, 그걸 리스트로 저장하는 역할을 담당합니다. wait는 task_struct 를 인자로 가지고, wait_queue_head에 리스트로 저장됩니다.
 */
extern wait_queue_head_t wq_rotlock;

/**
 * 로테이션 락이 저장되는 형태의 정의입니다.
 * .int type: 리드인지 라이트인지를 타입으로 넣고
 * .int degree: degree
 * .int range: range
 * .pid_t pid: 프로세스 아이디를 넣고
 * .int status: 락이 대기 상태인지, 락을 잡은(acquired) 상태인지 를 플래그로 기록합니다. wait_queue 에서 사용합니다.
 * .struct list_head list_node: 리스트 해드를 이용해, wait/acquire 리스트로 만들어 관리합니다. 플래그가 있기 떄문에 하나의 list_head 로도 구현이 가능합니다.
 */
typedef struct __rotlock_t {
	int type; // 0: read, 1: write
	int degree;
	int range;
	pid_t pid;
	int status;
	struct list_head list_node;
} rotlock_t;

/**
 *  deprecated. 현재 리스트들의 상태와 새로운 락을 비교하여 락을 잡을 수 있는지 여부를 판단합니다. 최초에는 필요하였으나 지금은 deprecated 되었습니다.
 */
int is_acquirable(rotlock_t *p_lock);

/**
 * 새로운 rotlock 을 pending 리스트에 등록합니다. 리스트의 구조에 대해서는 리스트 선언부 주석을 참고해주시기 바랍니다.
 */
int list_add_pending(rotlock_t *p_pending_lock);

/**
 * pending 리스트와 wait list 들을 재구성합니다. 현재 로테이션을 기준으로 하여, 로테이션을 포함하는 rotlock들은 waiting 으로, 로테이션을 포함하지 않는 것들은 pending 으로 보냅니다.
 */
int refresh_pending_waiting_lists(void);

/**
 * wait_write/read 에 있는 rotlock 들을 보고, acquire 될 수 있는 것이 있으면 acquire 리스트로 보냅니다.
 */
int wait_write_to_acquire(void);
int wait_read_to_acquire(void);

/**
 * 전체 리스트를 순환하며 하나라도 조건에 일치하는 락이 있으면 그것을 리스트에서 삭제합니다. unlock 시나 프로세스 exit 시 실행됩니다.
 */
int delete_lock(int type, int degree, int range, int pid);

/**
 * 디버깅용 리스트 프린트입니다. 매 시스템콜마다 현재 리스트 상태를 프린트해줍니다.
 */
void __print_all_lists(void);

int exit_rotlock(pid_t pid);
