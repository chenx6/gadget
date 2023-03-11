//! Arithmetic expression Parser
//!
//! ```plaintext
//! E  -> T E'
//! E' -> + T E'
//!     | - T E'
//!     | ε
//! T  -> V T'
//! T' -> * V T'
//!     | / V T'
//!     | ε
//! V  -> - V
//!     | V
//! F  -> i
//!     | ( E )
//! ```
use crate::lexer::{BracketType, Lexer, OperatorType, Token};

#[derive(Debug)]
pub enum ParseError {
    MissingBracket,
    Unmatch,
}

/// A recursive descent parser
pub struct Parser {
    lexer: Lexer,
}

impl Parser {
    pub fn new(lexer: Lexer) -> Parser {
        Parser { lexer }
    }

    pub fn parse(&mut self) -> Result<Node, ParseError> {
        self.e()
    }

    // E -> TE'
    fn e(&mut self) -> Result<Node, ParseError> {
        let t = self.t()?;
        self.equote(t)
    }

    // E' -> +TE'|-TE'|ε
    fn equote(&mut self, val: Node) -> Result<Node, ParseError> {
        // Select sentence by looking for FIRST+
        match self.lexer.peek() {
            // E' -> +TE'|-TE'
            Some(&Token::Operator(ref op @ (OperatorType::Plus | OperatorType::Sub))) => {
                let op = op.clone(); // Get op from reference
                self.lexer.next();
                let t = self.t()?;
                self.equote(Node::BinaryExpr {
                    op,
                    lhs: Box::new(val),
                    rhs: Box::new(t),
                })
            }
            // E' -> ε
            Some(&Token::Bracket(BracketType::Right)) | None => Ok(val),
            _ => Err(ParseError::Unmatch),
        }
    }

    // T -> VT'
    fn t(&mut self) -> Result<Node, ParseError> {
        let t = self.v()?;
        self.tquote(t)
    }

    // T' -> *VT'|/VT'|ε
    fn tquote(&mut self, val: Node) -> Result<Node, ParseError> {
        match self.lexer.peek() {
            // T' -> *FT'|/FT'
            Some(&Token::Operator(ref op @ (OperatorType::Mul | OperatorType::Div))) => {
                let op = op.clone();
                self.lexer.next();
                let v = self.v()?;
                self.tquote(Node::BinaryExpr {
                    op,
                    lhs: Box::new(val),
                    rhs: Box::new(v),
                })
            }
            // T' -> ε
            Some(
                &Token::Operator(OperatorType::Plus | OperatorType::Sub)
                | &Token::Bracket(BracketType::Right),
            )
            | None => Ok(val),
            _ => Err(ParseError::Unmatch),
        }
    }

    fn v(&mut self) -> Result<Node, ParseError> {
        match self.lexer.peek() {
            Some(&Token::Operator(OperatorType::Sub)) => {
                self.lexer.next();
                self.f(Some(OperatorType::Sub))
            }
            _ => self.f(None),
        }
    }

    // F -> i|(E)
    fn f(&mut self, val: Option<OperatorType>) -> Result<Node, ParseError> {
        match self.lexer.peek() {
            Some(&Token::Number(n)) => {
                self.lexer.next();
                let node = Node::Number(n);
                if let Some(OperatorType::Sub) = val {
                    Ok(Node::UnaryExpr {
                        op: OperatorType::Sub,
                        child: Box::new(node),
                    })
                } else {
                    Ok(node)
                }
            }
            Some(&Token::Bracket(BracketType::Left)) => {
                match self.lexer.next() {
                    Some(t) => t,
                    None => return Err(ParseError::Unmatch),
                };
                let e = self.e();
                match self.lexer.next() {
                    Some(&Token::Bracket(BracketType::Right)) => e,
                    _ => Err(ParseError::MissingBracket),
                }
            }
            _ => return Err(ParseError::Unmatch),
        }
    }
}

/// AST Node
#[derive(Debug, PartialEq)]
pub enum Node {
    Number(i32),
    UnaryExpr {
        op: OperatorType,
        child: Box<Node>,
    },
    BinaryExpr {
        op: OperatorType,
        lhs: Box<Node>,
        rhs: Box<Node>,
    },
}

#[cfg(test)]
mod test {
    use super::*;
    #[test]
    fn test_parser() {
        let lexer = Lexer::new("1+2*3").unwrap();
        let mut parser = Parser::new(lexer);
        let node = parser.parse();
        println!("{:?}", node);
        assert!(node.is_ok());
    }

    #[test]
    fn test_failed_parse() {
        let lexer = Lexer::new("1+2*").unwrap();
        let mut parser = Parser::new(lexer);
        let node = parser.parse();
        println!("{:?}", node);
        assert!(node.is_err());
    }

    #[test]
    fn test_unary() {
        let lexer = Lexer::new("-1+2").unwrap();
        let mut parser = Parser::new(lexer);
        let node = parser.parse();
        println!("{:?}", node);
        assert!(node.is_ok());
    }

    #[test]
    fn test_failed_unary() {
        let lexer = Lexer::new("--1+2").unwrap();
        let mut parser = Parser::new(lexer);
        let node = parser.parse();
        println!("{:?}", node);
        assert!(node.is_err());
    }
}
