/* Compile-time tests */
#include "wincc.h"

static Obj *current_fn;
static bool in_return = false;

static void check_expr(Node *node);
static void check_stmt(Node *node);

static void check_expr(Node *node)
{
    if (!node)
        return;

    switch (node->kind) {
    case ND_NULL_EXPR:
    case ND_NUM:
    case ND_VAR:
    case ND_VLA_PTR:
    case ND_MEMZERO:
    case ND_LABEL_VAL:
        return;

    case ND_NEG:
        return;
    case ND_NOT:
        return;
    case ND_BITNOT:
        return;
    case ND_MEMBER:
        return;
    case ND_DEREF:
        return;
    case ND_ADDR: {
        Node *nd = node->lhs;
        while (nd->kind == ND_MEMBER)
            nd = nd->lhs;
        if (nd->kind == ND_VAR && nd->var->is_local && nd->var->ty->kind != TY_VLA && in_return)
            error_tok(node->tok, "cannot return address of local variable");
        return;
    }

    case ND_CAST:
        check_expr(node->lhs);
        return;

    case ND_ASSIGN:
        check_expr(node->lhs);
        check_expr(node->rhs);
        return;

    case ND_COMMA:
        check_expr(node->lhs);
        check_expr(node->rhs);
        return;

    case ND_STMT_EXPR:
        for (Node *n = node->body; n; n = n->next)
        check_stmt(n);
        return;

    case ND_COND:
        check_expr(node->cond);
        check_expr(node->then);
        check_expr(node->els);
        return;

    case ND_LOGAND:
    case ND_LOGOR:
        check_expr(node->lhs);
        check_expr(node->rhs);
        return;

    case ND_FUNCALL:
        check_expr(node->lhs);
        for (Node *arg = node->args; arg; arg = arg->next)
            check_expr(arg);
        return;

    case ND_CAS:
        check_expr(node->cas_addr);
        check_expr(node->cas_old);
        check_expr(node->cas_new);
        return;

    case ND_EXCH:
        check_expr(node->lhs);
        check_expr(node->rhs);
        return;
    }

    check_expr(node->lhs);
    check_expr(node->rhs);

    switch (node->kind) {
    case ND_DIV:
    case ND_MOD:
        if (is_const_expr(node->rhs) && eval(node->rhs) == 0)
            error_tok(node->tok, "division by zero");
        return;
    }
}

static void check_stmt(Node *node)
{
    if (!node)
        return;

    switch (node->kind) {
    case ND_IF:
        check_expr(node->cond);
        check_stmt(node->then);
        check_stmt(node->els);
        return;

    case ND_FOR:
        check_stmt(node->init);
        check_expr(node->cond);
        check_stmt(node->then);
        check_expr(node->inc);
        return;

    case ND_DO:
        check_stmt(node->then);
        check_expr(node->cond);
        return;

    case ND_SWITCH:
        check_expr(node->cond);
        check_stmt(node->then);
        return;

    case ND_CASE:
    case ND_LABEL:
        check_stmt(node->lhs);
        return;

    case ND_BLOCK:
        for (Node *n = node->body; n; n = n->next)
            check_stmt(n);
        return;

    case ND_GOTO:
    case ND_ASM:
        return;

    case ND_GOTO_EXPR:
        check_expr(node->lhs);
        return;

    case ND_RETURN:
        in_return = true;
        check_expr(node->lhs);
        in_return = false;
        return;

    case ND_EXPR_STMT:
        check_expr(node->lhs);
        return;
    }

    error_tok(node->tok, "invalid statement");
}

void check_program(Obj *prog)
{
    for (Obj *fn = prog; fn; fn = fn->next) {
        if (!fn->is_function || !fn->is_definition)
            continue;

        current_fn = fn;
        check_stmt(fn->body);
    }
}