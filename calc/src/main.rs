mod lexer;
mod parser;

use lexer::{Lexer, OperatorType};
use parser::{Node, Parser};

/// Eval AST
fn eval(node: &Node) -> i32 {
    match node {
        Node::Number(n) => *n,
        Node::UnaryExpr { op, child } => {
            let child = eval(child);
            match op {
                OperatorType::Plus => child,
                OperatorType::Sub => -child,
                _ => child,
            }
        }
        Node::BinaryExpr { op, lhs, rhs } => {
            let lhs_ret = eval(lhs);
            let rhs_ret = eval(rhs);

            match op {
                OperatorType::Plus => lhs_ret + rhs_ret,
                OperatorType::Sub => lhs_ret - rhs_ret,
                OperatorType::Mul => lhs_ret * rhs_ret,
                OperatorType::Div => lhs_ret.checked_div(rhs_ret).unwrap_or(0),
            }
        }
    }
}

fn main() {
    let lexer = Lexer::new("(1+2)*3+4/2").expect("Failed to lex");
    println!("Tokens: {:?}", lexer);
    let mut parser = Parser::new(lexer);
    let ast = parser.parse().expect("Failed to parse");
    println!("AST: {:?}", ast);
    println!("Result: {}", eval(&ast));
}
