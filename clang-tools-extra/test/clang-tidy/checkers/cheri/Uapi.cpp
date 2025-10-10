// RUN: %check_clang_tidy %s cheri-Uapi %t

struct foo {
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>struct foo<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C32:CONVERT >>struct foo<< >>y x<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:1: warning: __C64:BEGIN >>struct foo<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:1: warning: __C64:CONVERT >>struct foo<< >>y x<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:8: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
	void *y;
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:TYPE >>__c32_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:2: warning: __C64:TYPE >>__c64_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:7: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:7: warning: __C64:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C32:FIELD >>foo_t<< >>y<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:8: warning: __C32:FIELD >>struct foo<< >>y<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-7]]:8: warning: __C64:FIELD >>foo_t<< >>y<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-8]]:8: warning: __C64:FIELD >>struct foo<< >>y<< >>Ptr<< [cheri-Uapi]
	void *x;
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:TYPE >>__c32_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:2: warning: __C64:TYPE >>__c64_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:7: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:7: warning: __C64:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C32:FIELD >>foo_t<< >>x<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:8: warning: __C32:FIELD >>struct foo<< >>x<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-7]]:8: warning: __C64:FIELD >>foo_t<< >>x<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-8]]:8: warning: __C64:FIELD >>struct foo<< >>x<< >>Ptr<< [cheri-Uapi]
};
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:END >>struct foo<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C64:END >>struct foo<< [cheri-Uapi]

typedef struct foo foo_t;
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>foo_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C32:CONVERT >>foo_t<< >>y x<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:1: warning: __C64:BEGIN >>foo_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:1: warning: __C64:CONVERT >>foo_t<< >>y x<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:16: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:16: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-7]]:20: warning: __C32:END >>foo_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-8]]:20: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-9]]:20: warning: __C64:END >>foo_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-10]]:20: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]


struct bar {
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>struct bar<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C32:CONVERT >>struct bar<< >>x data1<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:1: warning: __C64:BEGIN >>struct bar<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:1: warning: __C64:CONVERT >>struct bar<< >>x data1<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:8: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
	int x;
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: __C32:FIELD >>struct bar<< >>x<< >>Copy<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:6: warning: __C64:FIELD >>struct bar<< >>x<< >>Copy<< [cheri-Uapi]
	char data1[3+4] __attribute__((__aligned__((  __SIZEOF_POINTER__ * __alignof__(struct foo)))));
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: __C32:ARRAY >>struct bar<< >>data1<< >>Copy<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:7: warning: __C64:ARRAY >>struct bar<< >>data1<< >>Copy<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:88: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:88: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
};
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:END >>struct bar<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C64:END >>struct bar<< [cheri-Uapi]

struct baz {
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>struct baz<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C64:BEGIN >>struct baz<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:8: warning: Will not be able to convert this record [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:8: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
	int x;
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: __C32:FIELD >>struct baz<< >>x<< >>Copy<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:6: warning: __C64:FIELD >>struct baz<< >>x<< >>Copy<< [cheri-Uapi]
	char data2[sizeof(struct bar)][sizeof(foo_t)*7] __attribute__((__aligned__((  __builtin_offsetof(foo_t, x)))));
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: __C32:ARRAY >>struct baz<< >>data2<< >>Copy<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:7: warning: __C64:ARRAY >>struct baz<< >>data2<< >>Copy<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:27: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:27: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:40: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:40: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-7]]:99: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-8]]:99: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
};
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:END >>struct baz<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C64:END >>struct baz<< [cheri-Uapi]
typedef unsigned long __u64ptr;
struct x {
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>struct x<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C32:CONVERT >>struct x<< >>pp<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:1: warning: __C64:BEGIN >>struct x<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:1: warning: __C64:CONVERT >>struct x<< >>pp<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:8: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
	void * pp;
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:TYPE >>__c32_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:2: warning: __C64:TYPE >>__c64_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:7: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:7: warning: __C64:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:9: warning: __C32:FIELD >>struct x<< >>pp<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:9: warning: __C64:FIELD >>struct x<< >>pp<< >>Ptr<< [cheri-Uapi]
} __attribute__((__aligned__(sizeof(__u64ptr))));
// CHECK-MESSAGES: :[[@LINE-1]]:37: warning: __C64:TYPE >>__c64_ptr64<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:44: warning: __C64:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:46: warning: __C32:END >>struct x<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:46: warning: __C64:END >>struct x<< [cheri-Uapi]
typedef unsigned long long __u64;
typedef __u64 __attribute__((aligned(8))) __aligned_u64;

enum {
	FIRST_PTR,
	SECOND_PTR,
	NUM_PTRS
};

struct foo2 {
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>struct foo2<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C32:CONVERT >>struct foo2<< >>p<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:1: warning: __C64:BEGIN >>struct foo2<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:1: warning: __C64:CONVERT >>struct foo2<< >>p<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:8: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
	void *p[NUM_PTRS];
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:TYPE >>__c32_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:2: warning: __C64:TYPE >>__c64_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:7: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:7: warning: __C64:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C32:ARRAY >>struct foo2<< >>p<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:8: warning: __C64:ARRAY >>struct foo2<< >>p<< >>Ptr<< [cheri-Uapi]
};
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:END >>struct foo2<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C64:END >>struct foo2<< [cheri-Uapi]

int f(void)
{
	return 200 - 8 * NUM_PTRS;
}
typedef short __kernel_sa_family_t;
struct sockaddr {
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>struct sockaddr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C32:CONVERT >>struct sockaddr<< >>x<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:8: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
	unsigned long x;
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:TYPE >>__c32_ulong<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:14: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:16: warning: __C32:FIELD >>struct sockaddr<< >>x<< >>ULong<< [cheri-Uapi]
};
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:END >>struct sockaddr<< [cheri-Uapi]
struct sockaddr_in {
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>struct sockaddr_in<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C32:CONVERT >>struct sockaddr_in<< >>x<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:8: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
	unsigned long x;
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:TYPE >>__c32_ulong<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:14: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:16: warning: __C32:FIELD >>struct mptcp_subflow_addrs<< >>sin_local.x<< >>Ignore<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:16: warning: __C32:FIELD >>struct sockaddr_in<< >>x<< >>ULong<< [cheri-Uapi]
};
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:END >>struct sockaddr_in<< [cheri-Uapi]
struct sockaddr_in6 {
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>struct sockaddr_in6<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C32:CONVERT >>struct sockaddr_in6<< >>x<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:8: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
	unsigned long x;
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:TYPE >>__c32_ulong<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:14: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:16: warning: __C32:FIELD >>struct mptcp_subflow_addrs<< >>sin6_local.x<< >>Ignore<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:16: warning: __C32:FIELD >>struct sockaddr_in6<< >>x<< >>ULong<< [cheri-Uapi]
};
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:END >>struct sockaddr_in6<< [cheri-Uapi]
struct __kernel_sockaddr_storageX {
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>struct __kernel_sockaddr_storageX<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:8: warning: Will not be able to convert this record [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:8: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
	unsigned long x[128 / sizeof(unsigned long)];
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:TYPE >>__c32_ulong<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:14: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:16: warning: __C32:ARRAY >>struct __kernel_sockaddr_storageX<< >>x<< >>ULong<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:16: warning: __C32:ARRAY >>struct mptcp_subflow_addrs<< >>ss_local.x<< >>Ignore<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:31: warning: __C32:TYPE >>__c32_ulong<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:43: warning: __C32:ENDTYPE [cheri-Uapi]
};
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:END >>struct __kernel_sockaddr_storageX<< [cheri-Uapi]
struct mptcp_subflow_addrs {
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>struct mptcp_subflow_addrs<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:8: warning: Will not be able to convert this record [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:8: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
        union {
                /// UAPI: NoConvert: Convert ss_local instead
                __kernel_sa_family_t sa_family;
                /// UAPI: NoConvert: Convert ss_local instead
                struct sockaddr sa_local;
                /// UAPI: NoConvert: Convert ss_local instead
                struct sockaddr_in sin_local;
// CHECK-MESSAGES: :[[@LINE-1]]:24: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
                /// UAPI: NoConvert: Convert ss_local instead
                struct sockaddr_in6 sin6_local;
// CHECK-MESSAGES: :[[@LINE-1]]:24: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
                struct __kernel_sockaddr_storageX ss_local;
// CHECK-MESSAGES: :[[@LINE-1]]:24: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:51: warning: __C32:FIELD >>struct mptcp_subflow_addrs<< >>ss_local<< >>Ignore<< [cheri-Uapi]
        };
};
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:END >>struct mptcp_subflow_addrs<< [cheri-Uapi]

struct ok1 {
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>struct ok1<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C32:CONVERT >>struct ok1<< >>foo ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:1: warning: __C64:BEGIN >>struct ok1<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:1: warning: __C64:CONVERT >>struct ok1<< >>foo ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:8: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
	char foo[1];
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: __C32:ARRAY >>struct ok1<< >>foo<< >>Copy<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:7: warning: __C64:ARRAY >>struct ok1<< >>foo<< >>Copy<< [cheri-Uapi]
	void *ptr;
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:TYPE >>__c32_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:2: warning: __C64:TYPE >>__c64_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:7: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:7: warning: __C64:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C32:FIELD >>struct ok1<< >>ptr<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:8: warning: __C64:FIELD >>struct ok1<< >>ptr<< >>Ptr<< [cheri-Uapi]
};
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:END >>struct ok1<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C64:END >>struct ok1<< [cheri-Uapi]
struct ok2 {
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>struct ok2<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C32:CONVERT >>struct ok2<< >>foo ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:1: warning: __C64:BEGIN >>struct ok2<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:1: warning: __C64:CONVERT >>struct ok2<< >>foo ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:8: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
	char foo[0];
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: __C32:ARRAY >>struct ok2<< >>foo<< >>Copy<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:7: warning: __C64:ARRAY >>struct ok2<< >>foo<< >>Copy<< [cheri-Uapi]
	void *ptr;
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:TYPE >>__c32_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:2: warning: __C64:TYPE >>__c64_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:7: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:7: warning: __C64:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C32:FIELD >>struct ok2<< >>ptr<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:8: warning: __C64:FIELD >>struct ok2<< >>ptr<< >>Ptr<< [cheri-Uapi]
};
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:END >>struct ok2<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C64:END >>struct ok2<< [cheri-Uapi]
struct ok3 {
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>struct ok3<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C32:CONVERT >>struct ok3<< >>foo ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:1: warning: __C64:BEGIN >>struct ok3<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:1: warning: __C64:CONVERT >>struct ok3<< >>foo ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:8: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
	char foo[2];
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: __C32:ARRAY >>struct ok3<< >>foo<< >>Copy<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:7: warning: __C64:ARRAY >>struct ok3<< >>foo<< >>Copy<< [cheri-Uapi]
	void *ptr;
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:TYPE >>__c32_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:2: warning: __C64:TYPE >>__c64_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:7: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:7: warning: __C64:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C32:FIELD >>struct ok3<< >>ptr<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:8: warning: __C64:FIELD >>struct ok3<< >>ptr<< >>Ptr<< [cheri-Uapi]
};
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:END >>struct ok3<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C64:END >>struct ok3<< [cheri-Uapi]

struct bad1 {
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>struct bad1<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C64:BEGIN >>struct bad1<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:8: warning: Will not be able to convert this record [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:8: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
	void *ptr;
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:TYPE >>__c32_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:2: warning: __C64:TYPE >>__c64_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:7: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:7: warning: __C64:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C32:FIELD >>struct bad1<< >>ptr<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:8: warning: __C64:FIELD >>struct bad1<< >>ptr<< >>Ptr<< [cheri-Uapi]
	char foo[1];
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: __C32:ARRAY >>struct bad1<< >>foo<< >>Copy<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:7: warning: __C64:ARRAY >>struct bad1<< >>foo<< >>Copy<< [cheri-Uapi]
};
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:END >>struct bad1<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C64:END >>struct bad1<< [cheri-Uapi]
struct bad2 {
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>struct bad2<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C64:BEGIN >>struct bad2<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:8: warning: Will not be able to convert this record [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:8: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
	void *ptr;
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:TYPE >>__c32_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:2: warning: __C64:TYPE >>__c64_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:7: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:7: warning: __C64:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C32:FIELD >>struct bad2<< >>ptr<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:8: warning: __C64:FIELD >>struct bad2<< >>ptr<< >>Ptr<< [cheri-Uapi]
	char foo[0];
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: __C32:ARRAY >>struct bad2<< >>foo<< >>Copy<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:7: warning: __C64:ARRAY >>struct bad2<< >>foo<< >>Copy<< [cheri-Uapi]
};
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:END >>struct bad2<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C64:END >>struct bad2<< [cheri-Uapi]
struct ok4 {
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>struct ok4<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C32:CONVERT >>struct ok4<< >>ptr foo<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:1: warning: __C64:BEGIN >>struct ok4<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:1: warning: __C64:CONVERT >>struct ok4<< >>ptr foo<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:8: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
	void *ptr;
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:TYPE >>__c32_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:2: warning: __C64:TYPE >>__c64_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:7: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:7: warning: __C64:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C32:FIELD >>struct ok4<< >>ptr<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:8: warning: __C64:FIELD >>struct ok4<< >>ptr<< >>Ptr<< [cheri-Uapi]
	char foo[2];
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: __C32:ARRAY >>struct ok4<< >>foo<< >>Copy<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:7: warning: __C64:ARRAY >>struct ok4<< >>foo<< >>Copy<< [cheri-Uapi]
};
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:END >>struct ok4<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C64:END >>struct ok4<< [cheri-Uapi]

struct msg {
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>struct msg<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C32:CONVERT >>struct msg<< >>a b c<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:8: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
	long	a;
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:TYPE >>__c32_ulong<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:5: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:7: warning: __C32:FIELD >>struct msg<< >>a<< >>ULong<< [cheri-Uapi]
	long	b;
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:TYPE >>__c32_ulong<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:5: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:7: warning: __C32:FIELD >>struct msg<< >>b<< >>ULong<< [cheri-Uapi]
	long	c;
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:TYPE >>__c32_ulong<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:5: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:7: warning: __C32:FIELD >>struct msg<< >>c<< >>ULong<< [cheri-Uapi]
};
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:END >>struct msg<< [cheri-Uapi]

struct foo3 {
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>struct foo3<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C32:CONVERT >>struct foo3<< >>x<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:1: warning: __C64:BEGIN >>struct foo3<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:1: warning: __C64:CONVERT >>struct foo3<< >>x<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:8: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
	unsigned long x;
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:TYPE >>__c32_ulong<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:14: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:16: warning: __C32:FIELD >>struct foo3<< >>x<< >>ULong<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:16: warning: __C64:FIELD >>struct foo3<< >>x<< >>ULong<< [cheri-Uapi]
	union {
		/// UAPI: NoConvert: Kernel only
		void *ptr;
// CHECK-MESSAGES: :[[@LINE-1]]:3: warning: __C32:TYPE >>__c32_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:3: warning: __C64:TYPE >>__c64_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:8: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:8: warning: __C64:ENDTYPE [cheri-Uapi]
		/// UAPI: NoConvert: Kernel only
		unsigned long addr;
// CHECK-MESSAGES: :[[@LINE-1]]:3: warning: __C32:TYPE >>__c32_ulong<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:15: warning: __C32:ENDTYPE [cheri-Uapi]
	};
};
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:END >>struct foo3<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C64:END >>struct foo3<< [cheri-Uapi]

typedef struct foo4 {
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>x_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C32:BEGIN >>y_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:1: warning: __C32:CONVERT >>x_t<< >>ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:1: warning: __C32:CONVERT >>y_t<< >>ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:1: warning: __C64:BEGIN >>x_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:1: warning: __C64:BEGIN >>y_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-7]]:1: warning: __C64:CONVERT >>x_t<< >>ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-8]]:1: warning: __C64:CONVERT >>y_t<< >>ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-9]]:9: warning: __C32:BEGIN >>struct foo4<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-10]]:9: warning: __C32:CONVERT >>struct foo4<< >>ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-11]]:9: warning: __C64:BEGIN >>struct foo4<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-12]]:9: warning: __C64:CONVERT >>struct foo4<< >>ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-13]]:16: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-14]]:16: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
	void *ptr;
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:TYPE >>__c32_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:2: warning: __C64:TYPE >>__c64_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:7: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:7: warning: __C64:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C32:FIELD >>struct foo4<< >>ptr<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:8: warning: __C32:FIELD >>x_t<< >>ptr<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-7]]:8: warning: __C32:FIELD >>y_t<< >>ptr<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-8]]:8: warning: __C32:FIELD >>z_t<< >>ptr<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-9]]:8: warning: __C64:FIELD >>struct foo4<< >>ptr<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-10]]:8: warning: __C64:FIELD >>x_t<< >>ptr<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-11]]:8: warning: __C64:FIELD >>y_t<< >>ptr<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-12]]:8: warning: __C64:FIELD >>z_t<< >>ptr<< >>Ptr<< [cheri-Uapi]
} x_t, y_t;
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:END >>struct foo4<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C64:END >>struct foo4<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:3: warning: __C32:END >>x_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:3: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:3: warning: __C64:END >>x_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:3: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-7]]:8: warning: __C32:END >>y_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-8]]:8: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-9]]:8: warning: __C64:END >>y_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-10]]:8: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]

struct bar2 {
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>struct bar2<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C32:CONVERT >>struct bar2<< >>p2<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:1: warning: __C64:BEGIN >>struct bar2<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:1: warning: __C64:CONVERT >>struct bar2<< >>p2<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:8: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
	void *p2;
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:TYPE >>__c32_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:2: warning: __C64:TYPE >>__c64_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:7: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:7: warning: __C64:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C32:FIELD >>struct bar2<< >>p2<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:8: warning: __C64:FIELD >>struct bar2<< >>p2<< >>Ptr<< [cheri-Uapi]
};
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:END >>struct bar2<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C64:END >>struct bar2<< [cheri-Uapi]

typedef struct foo4 z_t;
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>z_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C32:CONVERT >>z_t<< >>ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:1: warning: __C64:BEGIN >>z_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:1: warning: __C64:CONVERT >>z_t<< >>ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:16: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:16: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-7]]:21: warning: __C32:END >>z_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-8]]:21: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-9]]:21: warning: __C64:END >>z_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-10]]:21: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
#define __user
struct cdrom_generic_command
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>struct cdrom_generic_command<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C32:CONVERT >>struct cdrom_generic_command<< >>cmd buffer buflen stat sense data_direction quiet timeout reserved<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:1: warning: __C64:BEGIN >>struct cdrom_generic_command<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:1: warning: __C64:CONVERT >>struct cdrom_generic_command<< >>cmd buffer buflen stat sense data_direction quiet timeout reserved<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:8: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
{
        unsigned char           cmd[16];
// CHECK-MESSAGES: :[[@LINE-1]]:33: warning: __C32:ARRAY >>struct cdrom_generic_command<< >>cmd<< >>Copy<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:33: warning: __C64:ARRAY >>struct cdrom_generic_command<< >>cmd<< >>Copy<< [cheri-Uapi]
        unsigned char           *buffer;
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: __C32:TYPE >>__c32_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:9: warning: __C64:TYPE >>__c64_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:33: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:33: warning: __C64:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:34: warning: __C32:FIELD >>struct cdrom_generic_command<< >>buffer<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:34: warning: __C64:FIELD >>struct cdrom_generic_command<< >>buffer<< >>Ptr<< [cheri-Uapi]
        unsigned int            buflen;
// CHECK-MESSAGES: :[[@LINE-1]]:33: warning: __C32:FIELD >>struct cdrom_generic_command<< >>buflen<< >>Copy<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:33: warning: __C64:FIELD >>struct cdrom_generic_command<< >>buflen<< >>Copy<< [cheri-Uapi]
        int                     stat;
// CHECK-MESSAGES: :[[@LINE-1]]:33: warning: __C32:FIELD >>struct cdrom_generic_command<< >>stat<< >>Copy<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:33: warning: __C64:FIELD >>struct cdrom_generic_command<< >>stat<< >>Copy<< [cheri-Uapi]
        void __user *sense;
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: __C32:TYPE >>__c32_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:9: warning: __C64:TYPE >>__c64_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:21: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:21: warning: __C64:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:22: warning: __C32:FIELD >>struct cdrom_generic_command<< >>sense<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:22: warning: __C64:FIELD >>struct cdrom_generic_command<< >>sense<< >>Ptr<< [cheri-Uapi]
        unsigned char           data_direction;
// CHECK-MESSAGES: :[[@LINE-1]]:33: warning: __C32:FIELD >>struct cdrom_generic_command<< >>data_direction<< >>Copy<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:33: warning: __C64:FIELD >>struct cdrom_generic_command<< >>data_direction<< >>Copy<< [cheri-Uapi]
        int                     quiet;
// CHECK-MESSAGES: :[[@LINE-1]]:33: warning: __C32:FIELD >>struct cdrom_generic_command<< >>quiet<< >>Copy<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:33: warning: __C64:FIELD >>struct cdrom_generic_command<< >>quiet<< >>Copy<< [cheri-Uapi]
        int                     timeout;
// CHECK-MESSAGES: :[[@LINE-1]]:33: warning: __C32:FIELD >>struct cdrom_generic_command<< >>timeout<< >>Copy<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:33: warning: __C64:FIELD >>struct cdrom_generic_command<< >>timeout<< >>Copy<< [cheri-Uapi]
        union {
                void            *reserved;    /* unused, actually */
// CHECK-MESSAGES: :[[@LINE-1]]:17: warning: __C32:TYPE >>__c32_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:17: warning: __C64:TYPE >>__c64_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:33: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:33: warning: __C64:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:34: warning: __C32:FIELD >>struct cdrom_generic_command<< >>reserved<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:34: warning: __C64:FIELD >>struct cdrom_generic_command<< >>reserved<< >>Ptr<< [cheri-Uapi]
                void            *unused;
// CHECK-MESSAGES: :[[@LINE-1]]:17: warning: __C32:TYPE >>__c32_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:17: warning: __C64:TYPE >>__c64_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:33: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:33: warning: __C64:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:34: warning: __C32:FIELD >>struct cdrom_generic_command<< >>unused<< >>Ignore<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:34: warning: __C64:FIELD >>struct cdrom_generic_command<< >>unused<< >>Ignore<< [cheri-Uapi]
        };
};
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:END >>struct cdrom_generic_command<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C64:END >>struct cdrom_generic_command<< [cheri-Uapi]


struct _x {
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>struct _x<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C32:CONVERT >>struct _x<< >>a b ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:1: warning: __C64:BEGIN >>struct _x<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:1: warning: __C64:CONVERT >>struct _x<< >>a b ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:8: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
	unsigned long a;
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:TYPE >>__c32_ulong<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:14: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:16: warning: __C32:FIELD >>struct _x<< >>a<< >>ULong<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:16: warning: __C64:FIELD >>struct _x<< >>a<< >>ULong<< [cheri-Uapi]
	unsigned long b;
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:TYPE >>__c32_ulong<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:14: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:16: warning: __C32:FIELD >>struct _x<< >>b<< >>ULong<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:16: warning: __C64:FIELD >>struct _x<< >>b<< >>ULong<< [cheri-Uapi]
	void *ptr __attribute__((aligned(8)));
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:TYPE >>__c32_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:2: warning: __C64:TYPE >>__c64_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:7: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:7: warning: __C64:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C32:FIELD >>struct _x<< >>ptr<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:8: warning: __C64:FIELD >>struct _x<< >>ptr<< >>Ptr<< [cheri-Uapi]
};
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:END >>struct _x<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C64:END >>struct _x<< [cheri-Uapi]

struct sigcation {
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: Will not be able to annotate this record [cheri-Uapi]
	void (*sa_handler)(int);
};

typedef unsigned long __u64ptr;
typedef unsigned char __u8;
#define __statfs_word unsigned long
#define __myptr void *
typedef struct {
	int	x[2];
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: __C32:ARRAY >>struct statfs64<< >>f_fsid.x<< >>Copy<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:6: warning: __C64:ARRAY >>struct statfs64<< >>f_fsid.x<< >>Copy<< [cheri-Uapi]
} __kernel_fsid_t;

struct statfs64 {
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>struct statfs64<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C32:CONVERT >>struct statfs64<< >>f_type f_bsize f_blocks f_bfree f_bavail f_files f_ffree f_fsid.x f_namelen f_frsize f_flags f_spare p<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:1: warning: __C64:BEGIN >>struct statfs64<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:1: warning: __C64:CONVERT >>struct statfs64<< >>f_type f_bsize f_blocks f_bfree f_bavail f_files f_ffree f_fsid.x f_namelen f_frsize f_flags f_spare p<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:8: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
        __statfs_word f_type;
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: __C32:TYPE >>__c32_ulong<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:21: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:23: warning: __C32:FIELD >>struct statfs64<< >>f_type<< >>ULong<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:23: warning: __C64:FIELD >>struct statfs64<< >>f_type<< >>ULong<< [cheri-Uapi]
        __statfs_word f_bsize;
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: __C32:TYPE >>__c32_ulong<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:21: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:23: warning: __C32:FIELD >>struct statfs64<< >>f_bsize<< >>ULong<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:23: warning: __C64:FIELD >>struct statfs64<< >>f_bsize<< >>ULong<< [cheri-Uapi]
        __u64 f_blocks;
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: __C32:FIELD >>struct statfs64<< >>f_blocks<< >>Copy<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:15: warning: __C64:FIELD >>struct statfs64<< >>f_blocks<< >>Copy<< [cheri-Uapi]
        __u64 f_bfree;
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: __C32:FIELD >>struct statfs64<< >>f_bfree<< >>Copy<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:15: warning: __C64:FIELD >>struct statfs64<< >>f_bfree<< >>Copy<< [cheri-Uapi]
        __u64 f_bavail;
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: __C32:FIELD >>struct statfs64<< >>f_bavail<< >>Copy<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:15: warning: __C64:FIELD >>struct statfs64<< >>f_bavail<< >>Copy<< [cheri-Uapi]
        __u64 f_files;
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: __C32:FIELD >>struct statfs64<< >>f_files<< >>Copy<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:15: warning: __C64:FIELD >>struct statfs64<< >>f_files<< >>Copy<< [cheri-Uapi]
        __u64 f_ffree;
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: __C32:FIELD >>struct statfs64<< >>f_ffree<< >>Copy<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:15: warning: __C64:FIELD >>struct statfs64<< >>f_ffree<< >>Copy<< [cheri-Uapi]
        __kernel_fsid_t f_fsid;
// CHECK-MESSAGES: :[[@LINE-1]]:25: warning: __C32:FIELD >>struct statfs64<< >>f_fsid<< >>Ignore<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:25: warning: __C64:FIELD >>struct statfs64<< >>f_fsid<< >>Ignore<< [cheri-Uapi]
        __statfs_word f_namelen;
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: __C32:TYPE >>__c32_ulong<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:21: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:23: warning: __C32:FIELD >>struct statfs64<< >>f_namelen<< >>ULong<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:23: warning: __C64:FIELD >>struct statfs64<< >>f_namelen<< >>ULong<< [cheri-Uapi]
        __statfs_word f_frsize;
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: __C32:TYPE >>__c32_ulong<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:21: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:23: warning: __C32:FIELD >>struct statfs64<< >>f_frsize<< >>ULong<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:23: warning: __C64:FIELD >>struct statfs64<< >>f_frsize<< >>ULong<< [cheri-Uapi]
        __statfs_word f_flags;
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: __C32:TYPE >>__c32_ulong<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:21: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:23: warning: __C32:FIELD >>struct statfs64<< >>f_flags<< >>ULong<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:23: warning: __C64:FIELD >>struct statfs64<< >>f_flags<< >>ULong<< [cheri-Uapi]
        __statfs_word f_spare[4];
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: __C32:TYPE >>__c32_ulong<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:21: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:23: warning: __C32:ARRAY >>struct statfs64<< >>f_spare<< >>ULong<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:23: warning: __C64:ARRAY >>struct statfs64<< >>f_spare<< >>ULong<< [cheri-Uapi]
	__myptr p;
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:TYPE >>__c32_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:2: warning: __C64:TYPE >>__c64_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:8: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:8: warning: __C64:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:10: warning: __C32:FIELD >>struct statfs64<< >>p<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:10: warning: __C64:FIELD >>struct statfs64<< >>p<< >>Ptr<< [cheri-Uapi]
};
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:END >>struct statfs64<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C64:END >>struct statfs64<< [cheri-Uapi]

typedef struct {
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>xnoname_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C32:CONVERT >>xnoname_t<< >>p<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:1: warning: __C64:BEGIN >>xnoname_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:1: warning: __C64:CONVERT >>xnoname_t<< >>p<< [cheri-Uapi]
	void *p;
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:TYPE >>__c32_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:2: warning: __C64:TYPE >>__c64_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:7: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:7: warning: __C64:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C32:FIELD >>struct x2<< >>noname.p<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:8: warning: __C32:FIELD >>xnoname_t<< >>p<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-7]]:8: warning: __C64:FIELD >>struct x2<< >>noname.p<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-8]]:8: warning: __C64:FIELD >>xnoname_t<< >>p<< >>Ptr<< [cheri-Uapi]
} xnoname_t;
// CHECK-MESSAGES: :[[@LINE-1]]:3: warning: __C32:END >>xnoname_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:3: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:3: warning: __C64:END >>xnoname_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:3: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]

typedef struct xname {
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>xname_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C32:CONVERT >>xname_t<< >>p<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:1: warning: __C64:BEGIN >>xname_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:1: warning: __C64:CONVERT >>xname_t<< >>p<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:9: warning: __C32:BEGIN >>struct xname<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:9: warning: __C32:CONVERT >>struct xname<< >>p<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-7]]:9: warning: __C64:BEGIN >>struct xname<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-8]]:9: warning: __C64:CONVERT >>struct xname<< >>p<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-9]]:16: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-10]]:16: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
	void *p;
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:TYPE >>__c32_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:2: warning: __C64:TYPE >>__c64_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:7: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:7: warning: __C64:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C32:FIELD >>struct x2<< >>name.p<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:8: warning: __C32:FIELD >>struct x2<< >>out_of_line.p<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-7]]:8: warning: __C32:FIELD >>struct xname<< >>p<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-8]]:8: warning: __C32:FIELD >>xname_t<< >>p<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-9]]:8: warning: __C64:FIELD >>struct x2<< >>name.p<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-10]]:8: warning: __C64:FIELD >>struct x2<< >>out_of_line.p<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-11]]:8: warning: __C64:FIELD >>struct xname<< >>p<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-12]]:8: warning: __C64:FIELD >>xname_t<< >>p<< >>Ptr<< [cheri-Uapi]
} xname_t;
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:END >>struct xname<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C64:END >>struct xname<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:3: warning: __C32:END >>xname_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:3: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:3: warning: __C64:END >>xname_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:3: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]

struct x2 {
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>struct x2<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C32:CONVERT >>struct x2<< >>noname.p name.p xnoname_inline.p xname_inline.p xname_inline.q n2.p n2.q out_of_line.p<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:1: warning: __C64:BEGIN >>struct x2<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:1: warning: __C64:CONVERT >>struct x2<< >>noname.p name.p xnoname_inline.p xname_inline.p xname_inline.q n2.p n2.q out_of_line.p<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:8: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
	xnoname_t noname;
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:2: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:12: warning: __C32:FIELD >>struct x2<< >>noname<< >>Ignore<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:12: warning: __C64:FIELD >>struct x2<< >>noname<< >>Ignore<< [cheri-Uapi]
	const xname_t const name;
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:8: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:22: warning: __C32:FIELD >>struct x2<< >>name<< >>Ignore<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:22: warning: __C64:FIELD >>struct x2<< >>name<< >>Ignore<< [cheri-Uapi]
	struct {
		void *p;
// CHECK-MESSAGES: :[[@LINE-1]]:3: warning: __C32:TYPE >>__c32_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:3: warning: __C64:TYPE >>__c64_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:8: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:8: warning: __C64:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:9: warning: __C32:FIELD >>struct x2<< >>xnoname_inline.p<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:9: warning: __C64:FIELD >>struct x2<< >>xnoname_inline.p<< >>Ptr<< [cheri-Uapi]
	} xnoname_inline;
// CHECK-MESSAGES: :[[@LINE-1]]:4: warning: __C32:FIELD >>struct x2<< >>xnoname_inline<< >>Ignore<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:4: warning: __C64:FIELD >>struct x2<< >>xnoname_inline<< >>Ignore<< [cheri-Uapi]
	struct inline_name {
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:BEGIN >>struct inline_name<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:2: warning: __C32:CONVERT >>struct inline_name<< >>p q<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:2: warning: __C64:BEGIN >>struct inline_name<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:2: warning: __C64:CONVERT >>struct inline_name<< >>p q<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:9: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:9: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
		void *p, *q;
// CHECK-MESSAGES: :[[@LINE-1]]:3: warning: __C32:TYPE >>__c32_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:3: warning: __C64:TYPE >>__c64_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:8: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:8: warning: __C64:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:9: warning: __C32:FIELD >>struct inline_name<< >>p<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:9: warning: __C32:FIELD >>struct x2<< >>n2.p<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-7]]:9: warning: __C32:FIELD >>struct x2<< >>xname_inline.p<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-8]]:9: warning: __C64:FIELD >>struct inline_name<< >>p<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-9]]:9: warning: __C64:FIELD >>struct x2<< >>n2.p<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-10]]:9: warning: __C64:FIELD >>struct x2<< >>xname_inline.p<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-11]]:13: warning: __C32:FIELD >>struct inline_name<< >>q<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-12]]:13: warning: __C32:FIELD >>struct x2<< >>n2.q<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-13]]:13: warning: __C32:FIELD >>struct x2<< >>xname_inline.q<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-14]]:13: warning: __C64:FIELD >>struct inline_name<< >>q<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-15]]:13: warning: __C64:FIELD >>struct x2<< >>n2.q<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-16]]:13: warning: __C64:FIELD >>struct x2<< >>xname_inline.q<< >>Ptr<< [cheri-Uapi]
	} xname_inline, n2;
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:END >>struct inline_name<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:2: warning: __C64:END >>struct inline_name<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:4: warning: __C32:FIELD >>struct x2<< >>xname_inline<< >>Ignore<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:4: warning: __C64:FIELD >>struct x2<< >>xname_inline<< >>Ignore<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:18: warning: __C32:FIELD >>struct x2<< >>n2<< >>Ignore<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:18: warning: __C64:FIELD >>struct x2<< >>n2<< >>Ignore<< [cheri-Uapi]
	struct xname out_of_line;
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:9: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:15: warning: __C32:FIELD >>struct x2<< >>out_of_line<< >>Ignore<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:15: warning: __C64:FIELD >>struct x2<< >>out_of_line<< >>Ignore<< [cheri-Uapi]
};
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:END >>struct x2<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C64:END >>struct x2<< [cheri-Uapi]

#define OPAL_KEY_MAX 256
struct opal_key {
        __u8 lr;
// CHECK-MESSAGES: :[[@LINE-1]]:14: warning: __C64:FIELD >>struct opal_shadow_mbr<< >>key.lr<< >>Ignore<< [cheri-Uapi]
        __u8 key_len;
// CHECK-MESSAGES: :[[@LINE-1]]:14: warning: __C64:FIELD >>struct opal_shadow_mbr<< >>key.key_len<< >>Ignore<< [cheri-Uapi]
        __u8 key_type;
// CHECK-MESSAGES: :[[@LINE-1]]:14: warning: __C64:FIELD >>struct opal_shadow_mbr<< >>key.key_type<< >>Ignore<< [cheri-Uapi]
        __u8 __align[5];
// CHECK-MESSAGES: :[[@LINE-1]]:14: warning: __C64:ARRAY >>struct opal_shadow_mbr<< >>key.__align<< >>Ignore<< [cheri-Uapi]
        __u8 key[OPAL_KEY_MAX];
// CHECK-MESSAGES: :[[@LINE-1]]:14: warning: __C64:ARRAY >>struct opal_shadow_mbr<< >>key.key<< >>Ignore<< [cheri-Uapi]
};

struct opal_shadow_mbr {
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C64:BEGIN >>struct opal_shadow_mbr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C64:CONVERT >>struct opal_shadow_mbr<< >>key data data2 data3 offset size<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:8: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
        struct opal_key key;
// CHECK-MESSAGES: :[[@LINE-1]]:25: warning: __C64:FIELD >>struct opal_shadow_mbr<< >>key<< >>Memcopy<< [cheri-Uapi]
        const __u64ptr volatile data, data2, data3;
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: __C64:TYPE >>__c64_ptr64<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:31: warning: __C64:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:33: warning: __C64:FIELD >>struct opal_shadow_mbr<< >>data<< >>UPtr64<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:39: warning: __C64:FIELD >>struct opal_shadow_mbr<< >>data2<< >>UPtr64<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:46: warning: __C64:FIELD >>struct opal_shadow_mbr<< >>data3<< >>UPtr64<< [cheri-Uapi]
        __u64 offset;
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: __C64:FIELD >>struct opal_shadow_mbr<< >>offset<< >>Copy<< [cheri-Uapi]
        __u64 size;
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: __C64:FIELD >>struct opal_shadow_mbr<< >>size<< >>Copy<< [cheri-Uapi]
};
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C64:END >>struct opal_shadow_mbr<< [cheri-Uapi]

typedef struct need_compat {
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>need_comapt_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C32:CONVERT >>need_comapt_t<< >>ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:1: warning: __C64:BEGIN >>need_comapt_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:1: warning: __C64:CONVERT >>need_comapt_t<< >>ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:9: warning: __C32:BEGIN >>struct need_compat<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:9: warning: __C32:CONVERT >>struct need_compat<< >>ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-7]]:9: warning: __C64:BEGIN >>struct need_compat<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-8]]:9: warning: __C64:CONVERT >>struct need_compat<< >>ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-9]]:16: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-10]]:16: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
	void *ptr;
// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: __C32:TYPE >>__c32_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:2: warning: __C64:TYPE >>__c64_uptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:7: warning: __C32:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:7: warning: __C64:ENDTYPE [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:8: warning: __C32:FIELD >>need_comapt_t<< >>ptr<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:8: warning: __C32:FIELD >>need_compat2_t<< >>ptr<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-7]]:8: warning: __C32:FIELD >>struct need_compat<< >>ptr<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-8]]:8: warning: __C64:FIELD >>need_comapt_t<< >>ptr<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-9]]:8: warning: __C64:FIELD >>need_compat2_t<< >>ptr<< >>Ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-10]]:8: warning: __C64:FIELD >>struct need_compat<< >>ptr<< >>Ptr<< [cheri-Uapi]
} need_comapt_t;
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:END >>struct need_compat<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C64:END >>struct need_compat<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:3: warning: __C32:END >>need_comapt_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:3: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:3: warning: __C64:END >>need_comapt_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:3: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]

typedef struct need_compat need_compat2_t;
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: __C32:BEGIN >>need_compat2_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-2]]:1: warning: __C32:CONVERT >>need_compat2_t<< >>ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-3]]:1: warning: __C64:BEGIN >>need_compat2_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-4]]:1: warning: __C64:CONVERT >>need_compat2_t<< >>ptr<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-5]]:16: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-6]]:16: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-7]]:28: warning: __C32:END >>need_compat2_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-8]]:28: warning: __C32:INSERT >>__c32_<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-9]]:28: warning: __C64:END >>need_compat2_t<< [cheri-Uapi]
// CHECK-MESSAGES: :[[@LINE-10]]:28: warning: __C64:INSERT >>__c64_<< [cheri-Uapi]

