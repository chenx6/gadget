#[derive(Debug)]
pub enum LexerError {
    NumberFormatError,
    UnknownChar,
    LexError,
}

#[derive(Debug, PartialEq, Clone)]
pub enum OperatorType {
    Plus,
    Sub,
    Div,
    Mul,
}

#[derive(Debug, PartialEq)]
pub enum BracketType {
    Left,
    Right,
}

#[derive(Debug, PartialEq)]
pub enum Token {
    Number(i32),
    Operator(OperatorType),
    Bracket(BracketType),
}

/// A hand-written lexer
#[derive(Debug)]
pub struct Lexer {
    tokens: Vec<Token>,
    pos: usize,
}

struct Scanner {
    raw: String,
    pos: usize,
}

impl Scanner {
    fn new(s: &str) -> Scanner {
        Scanner {
            raw: String::from(s),
            pos: 0,
        }
    }

    fn peek(&mut self) -> Option<char> {
        self.raw.chars().nth(self.pos)
    }

    fn next(&mut self) -> Option<char> {
        self.pos += 1;
        self.raw.chars().nth(self.pos - 1)
    }
}

impl Lexer {
    pub fn new(s: &str) -> Result<Lexer, LexerError> {
        let mut scanner = Scanner::new(s);
        let mut tokens = vec![];
        while let Some(ch) = scanner.peek() {
            match ch {
                // Operator
                '+' | '-' | '*' | '/' => {
                    let op = match ch {
                        '+' => OperatorType::Plus,
                        '-' => OperatorType::Sub,
                        '*' => OperatorType::Mul,
                        '/' => OperatorType::Div,
                        _ => return Err(LexerError::LexError),
                    };
                    tokens.push(Token::Operator(op));
                    scanner.next();
                }
                // Number
                '0'..='9' => {
                    let mut buf = String::new();
                    while let Some(ch) = scanner.peek() {
                        if !('0'..='9').contains(&ch) {
                            break;
                        }
                        buf.push(ch);
                        scanner.next();
                    }
                    tokens.push(Token::Number(
                        buf.parse::<i32>()
                            .map_err(|_| LexerError::NumberFormatError)?,
                    ))
                }
                // Bracket
                '(' | ')' => {
                    let br = match ch {
                        '(' => BracketType::Left,
                        ')' => BracketType::Right,
                        _ => return Err(LexerError::LexError),
                    };
                    tokens.push(Token::Bracket(br));
                    scanner.next();
                }
                // Skip blank
                ' ' => {
                    scanner.next();
                }
                _ => return Err(LexerError::UnknownChar),
            }
        }
        Ok(Lexer { tokens, pos: 0 })
    }

    pub fn next(&mut self) -> Option<&Token> {
        self.pos += 1;
        self.tokens.get(self.pos - 1)
    }

    pub fn peek(&mut self) -> Option<&Token> {
        self.tokens.get(self.pos)
    }
}

#[cfg(test)]
mod test {
    use super::*;
    #[test]
    fn test_lexer() {
        let mut lexer = Lexer::new("1+23 -4*( 567/8)").unwrap();
        assert_eq!(lexer.next(), Some(&Token::Number(1)));
        assert_eq!(lexer.next(), Some(&Token::Operator(OperatorType::Plus)));
        assert_eq!(lexer.next(), Some(&Token::Number(23)));
        assert_eq!(lexer.next(), Some(&Token::Operator(OperatorType::Sub)));
        assert_eq!(lexer.next(), Some(&Token::Number(4)));
        assert_eq!(lexer.next(), Some(&Token::Operator(OperatorType::Mul)));
        assert_eq!(lexer.next(), Some(&Token::Bracket(BracketType::Left)));
        assert_eq!(lexer.next(), Some(&Token::Number(567)));
        assert_eq!(lexer.next(), Some(&Token::Operator(OperatorType::Div)));
        assert_eq!(lexer.next(), Some(&Token::Number(8)));
        assert_eq!(lexer.next(), Some(&Token::Bracket(BracketType::Right)));
    }
}
