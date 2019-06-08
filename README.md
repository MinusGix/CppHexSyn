# C++ HexSyn
A recoding of my javascript (nodejs) HexSyn 'language' in C++. Is different in various ways.
Syntax is different, learning from it, and it allows parsing of arbitrarily large files.

This is a language meant for writing file formats (and for figuring out how they work).
Hex Editors are useful, but even the ones which let you write notes about certain parts of the files are lacking in simpler writing and reading of these files. (At least that's how I felt, and thus I made this).

Examples:
```
AABB CC 42 ; comments
```
When you run the program on a file like this it would write to the file the hex `AABBCC42`. The default number type of hex, but it also supports decimal (integers), octal, and binary.
Note: currently there is no method to write partial byte groups that 'clump' together.
So writing:
```
$b0101 A
```
Will *not* result in `5A` being written, but rather each item is extended to a full byte and this would result in `050A` (0b00000101 and A) being written (both individually extended to a full byte.)
It would be nice to have at least some simple method for this, but it is currently not possible.

Based on the above 'rule', we have:
```
ABC
```
Which is 3 half-byte items, this will be extended to `0ABC`.

## Number formats

Hex:
The default number format is hexadecimal, so you can write it without worry. In the cases where you need to, it is written as:
`$xAFBC` or if you want to specify a minimum resulting byte-size you would use a capital X, like:
`$X4_AFBC` means that it must be at least four bytes, and will be extended to that with 0's. Note that the 'min-byte-size' (the 4) is in decimal.
So `$X13_AFBC` means 'extend this to a minimum of 13 bytes'.

Binary:
Same as hex.
`$b0110` which would extend to `$b00000110`
There is also `$B3_0110` which would be the same as `$b000000000000000000000110`

Octal:
Not yet implemented. It would be likely fairly easy, but it hasn't been added yet.

Decimal:
Same as hex.
`$d421` which would be turned into bytes, extending to a full byte.
`$D4_421` Lets you make sure it has a min-byte size, extended by zeroes.

## Strings

Strings are only ascii (sadly), so if you want utf-8/16/whatever, good luck. It would be quite nice to have it, though.
Simple string:
`$s"Hi"` This would write `48 69 00`
Note that it ends with a null-byte, which I decided was reasonable default behavior.
Also of note is that you *cannot* use single quotes.
There is a limited amount of escape sequences built in, which will be gone over later.

### Special Flags
The string 'command' (I don't know of a better term) has some flags which can be used. It would be nice to have more and have them be more generic but this is what is currently implemented:
`$sz"Hi"` This *disables* the ending null byte. Output: `48 69`
`$sZ"Hi"` This makes it start with a null byte. Output: `00 48 69 00`. (Note: since the default behavior is to have a null byte at the end it will still be there. If you don't want the nb at the end, then do `$sZz"Hi"`. I agree, it's super readable /s)
`$sx"Hi"` This 'splits' each character with a null byte. Output: `48 00 69 00`. (Note: It does not add a null-byte after the last character, but since the default behavior is to add a null byte it doesn't. So, `$sxz"Hi"` would output `48 00 69`)

It would be nice to have one which enables it to automatically write the length before/after the string, since that's fairly common.

### Escapes
These are inside of strings.
`\n` - newline
`\t` - tab
`\{anything}` - Just repeats whatever `{anything}` is, so you can use it to do `$s"Hi\" yay"`
Would be nice to: Hex escape, octal escape, more escapes for common escapes (like \v and the like, pretty easy to add)

## Repeat
There is a repeat operator, which is useful.
`A1 * 6` becomes `A1 A1 A1 A1 A1 A1`
And should work on whatever you use it on. It only affects the last read data.
`AABB * 3` becomes `AABB AABB AABB`
While `AA BB * 3` becomes `AA BB BB BB`

## Blocks
Blocks allow you to encapsulate anything. Useful for organizing code into sections (or you could just indent).
The more useful part is that it allows you to do an effect on a lot of data.
The currently most useful method is using it with the repeat operator.
`{AA BB $s"Hi"} * 3` becomes `AA BB 48 69 00 AA BB 48 69 00 AA BB 48 69 00`