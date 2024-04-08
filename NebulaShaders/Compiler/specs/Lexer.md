# Specifications for the Lexer

## Lexer Specs
- Should never throw an exception (No allocations)

## Initialization
- Takes a stringview of a string 'source'

## Destruction
- There is no work to be done

## NextToken (void) -> Token
- Returns the next token (Special cases include EOF for end of file, Unknown for nonprintable character other than \n and \r)

## Remove Whitespace
- Removes all whitespace [\s\n\r\t]

## NextIdentifier
- There is the first character [a-zA-Z_]
- Read all characters
