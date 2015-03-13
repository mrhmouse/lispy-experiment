This is just a little toy I'm playing around with to familiarize
myself with C. I don't expect it to be useful or interesting to
anybody else.

Right now it just parses something similar to s-expressions.
The following items can be parsed:

- Integers: `1234`
  - Known bug: integer literals will silently overflow
- Doubles: `1234.5678`
- Symbols: `hello?`, `world!`, `"this is a \"quoted\" symbol"`
- Lists: `(1 2 3 4 (five six (seven eight)))`
