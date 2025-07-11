#ifndef LEPTJSON_H__
#define LEPTJSON_H__

#include <stddef.h> /* size_t */

/* JSON 数据类型枚举 */
typedef enum { LEPT_NULL, LEPT_FALSE, LEPT_TRUE, LEPT_NUMBER, LEPT_STRING, LEPT_ARRAY, LEPT_OBJECT } lept_type;

/* 查找对象成员时，未找到返回的常量 */
#define LEPT_KEY_NOT_EXIST ((size_t)-1)

/* 前置声明 */
typedef struct lept_value lept_value;
typedef struct lept_member lept_member;

/* JSON 值结构体 */
struct lept_value {
    union {
        struct { lept_member* m; size_t size, capacity; }o; /* 对象类型：成员数组、成员数量、容量 */
        struct { lept_value*  e; size_t size, capacity; }a; /* 数组类型：元素数组、元素数量、容量 */
        struct { char* s; size_t len; }s;                   /* 字符串类型：字符串指针、长度 */
        double n;                                           /* 数值类型 */
    }u;
    lept_type type; /* 当前值的类型 */
};

/* 对象成员结构体 */
struct lept_member {
    char* k; size_t klen;   /* 键值字符串及其长度 */
    lept_value v;           /* 键值对应的值 */
};

/* 解析返回码枚举 */
enum {
    LEPT_PARSE_OK = 0,                        /* 解析成功 */
    LEPT_PARSE_EXPECT_VALUE,                   /* 需要值 */
    LEPT_PARSE_INVALID_VALUE,                  /* 无效值 */
    LEPT_PARSE_ROOT_NOT_SINGULAR,              /* 根非单一元素 */
    LEPT_PARSE_NUMBER_TOO_BIG,                 /* 数字过大 */
    LEPT_PARSE_MISS_QUOTATION_MARK,            /* 缺少引号 */
    LEPT_PARSE_INVALID_STRING_ESCAPE,          /* 无效的字符串转义 */
    LEPT_PARSE_INVALID_STRING_CHAR,            /* 无效的字符串字符 */
    LEPT_PARSE_INVALID_UNICODE_HEX,            /* 无效的 Unicode 十六进制 */
    LEPT_PARSE_INVALID_UNICODE_SURROGATE,      /* 无效的 Unicode 代理对 */
    LEPT_PARSE_MISS_COMMA_OR_SQUARE_BRACKET,   /* 缺少逗号或方括号 */
    LEPT_PARSE_MISS_KEY,                       /* 缺少对象键 */
    LEPT_PARSE_MISS_COLON,                     /* 缺少冒号 */
    LEPT_PARSE_MISS_COMMA_OR_CURLY_BRACKET     /* 缺少逗号或大括号 */
};

/* 初始化 lept_value，将类型设为 LEPT_NULL */
#define lept_init(v) do { (v)->type = LEPT_NULL; } while(0)

/* 解析 JSON 字符串，结果存储到 lept_value 结构体中 */
int lept_parse(lept_value* v, const char* json);
/* 将 lept_value 结构体序列化为 JSON 字符串，返回字符串指针和长度 */
char* lept_stringify(const lept_value* v, size_t* length);

/* 拷贝、移动、交换 lept_value */
void lept_copy(lept_value* dst, const lept_value* src);
void lept_move(lept_value* dst, lept_value* src);
void lept_swap(lept_value* lhs, lept_value* rhs);

/* 释放 lept_value 结构体中分配的内存 */
void lept_free(lept_value* v);

/* 获取 lept_value 的类型 */
lept_type lept_get_type(const lept_value* v);
/* 比较两个 lept_value 是否相等 */
int lept_is_equal(const lept_value* lhs, const lept_value* rhs);

/* 将 lept_value 设为 null（即释放其内容） */
#define lept_set_null(v) lept_free(v)

/* 获取/设置布尔值，LEPT_TRUE 返回 1，LEPT_FALSE 返回 0 */
int lept_get_boolean(const lept_value* v);
void lept_set_boolean(lept_value* v, int b);

/* 获取/设置数值 */
double lept_get_number(const lept_value* v);
void lept_set_number(lept_value* v, double n);

/* 获取/设置字符串及其长度 */
const char* lept_get_string(const lept_value* v);
size_t lept_get_string_length(const lept_value* v);
void lept_set_string(lept_value* v, const char* s, size_t len);

/* 数组相关操作 */
void lept_set_array(lept_value* v, size_t capacity);                /* 设置为数组类型并分配容量 */
size_t lept_get_array_size(const lept_value* v);                    /* 获取数组元素数量 */
size_t lept_get_array_capacity(const lept_value* v);                /* 获取数组容量 */
void lept_reserve_array(lept_value* v, size_t capacity);            /* 预留数组容量 */
void lept_shrink_array(lept_value* v);                              /* 收缩数组容量到实际大小 */
void lept_clear_array(lept_value* v);                               /* 清空数组 */
lept_value* lept_get_array_element(lept_value* v, size_t index);    /* 获取指定下标的元素 */
lept_value* lept_pushback_array_element(lept_value* v);             /* 在数组末尾添加元素 */
void lept_popback_array_element(lept_value* v);                     /* 移除数组末尾元素 */
lept_value* lept_insert_array_element(lept_value* v, size_t index); /* 在指定位置插入元素 */
void lept_erase_array_element(lept_value* v, size_t index, size_t count); /* 删除指定范围的元素 */

/* 对象相关操作 */
void lept_set_object(lept_value* v, size_t capacity);               /* 设置为对象类型并分配容量 */
size_t lept_get_object_size(const lept_value* v);                   /* 获取对象成员数量 */
size_t lept_get_object_capacity(const lept_value* v);               /* 获取对象容量 */
void lept_reserve_object(lept_value* v, size_t capacity);           /* 预留对象容量 */
void lept_shrink_object(lept_value* v);                             /* 收缩对象容量到实际大小 */
void lept_clear_object(lept_value* v);                              /* 清空对象 */
const char* lept_get_object_key(const lept_value* v, size_t index); /* 获取指定成员的键 */
size_t lept_get_object_key_length(const lept_value* v, size_t index); /* 获取指定成员键的长度 */
lept_value* lept_get_object_value(lept_value* v, size_t index);     /* 获取指定成员的值 */
size_t lept_find_object_index(const lept_value* v, const char* key, size_t klen); /* 查找键对应的下标 */
lept_value* lept_find_object_value(lept_value* v, const char* key, size_t klen);  /* 查找键对应的值 */
lept_value* lept_set_object_value(lept_value* v, const char* key, size_t klen);   /* 设置键对应的值 */
void lept_remove_object_value(lept_value* v, size_t index);         /* 移除指定下标的成员 */

#endif /* LEPTJSON_H__ */
