#include "AstTree.h"
using namespace xcool::ast;

void AssExpr::accept_visitor(ExprVisitor &visitor)   { visitor.visit(*this); }
void UnaryExpr::accept_visitor(ExprVisitor &visitor) { visitor.visit(*this); }
void DisExpr::accept_visitor(ExprVisitor &visitor) { visitor.visit(*this); }
void BlockExpr::accept_visitor(ExprVisitor &visitor) { visitor.visit(*this); }
void IntExpr::accept_visitor(ExprVisitor &visitor) { visitor.visit(*this); }
void BoolExpr::accept_visitor(ExprVisitor &visitor) { visitor.visit(*this); }
void StrExpr::accept_visitor(ExprVisitor &visitor) { visitor.visit(*this); }
void LetExpr::accept_visitor(ExprVisitor &visitor) { visitor.visit(*this); }
void NewExpr::accept_visitor(ExprVisitor &visitor) { visitor.visit(*this); }
void IdExpr::accept_visitor(ExprVisitor &visitor) { visitor.visit(*this); }
void WhileExpr::accept_visitor(ExprVisitor &visitor) { visitor.visit(*this); }
void IfExpr::accept_visitor(ExprVisitor &visitor) { visitor.visit(*this); }
void BinExpr::accept_visitor(ExprVisitor &visitor) { visitor.visit(*this); }
void CaseExpr::accept_visitor(ExprVisitor &visitor) { visitor.visit(*this); }
