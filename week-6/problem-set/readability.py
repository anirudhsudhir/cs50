# TODO


def main():
    text = input("Text: ")
    grade = calculate_grade(count_letters(text), count_words(text), count_sen(text))
    if grade < 1:
        print("Before Grade 1")
    elif grade >= 16:
        print("Grade 16+")
    else:
        print("Grade ", grade)
    print("\n")


def count_letters(text):
    letters = 0
    i = 0
    while i < len(text):
        if text[i].isalpha():
            letters += 1
        i += 1
    return letters


def count_words(text):
    words = 1
    i = 0
    while i < len(text):
        if text[i] == " ":
            words += 1
        i += 1
    return words


def count_sen(text):
    sentences = 0
    i = 0
    while i < len(text):
        tempchar = text[i]
        if tempchar == "?" or tempchar == "." or tempchar == "!":
            sentences += 1
        i += 1
    return sentences


def calculate_grade(letters, words, sen):
    L = letters / words * 100
    S = sen / words * 100
    index = round(0.0588 * L - 0.296 * S - 15.8)
    return index


main()
