from enum import Enum, auto
from functools import cmp_to_key

class HandType(Enum):
    FIVE = auto()
    FOUR = auto()
    FULL_HOUSE = auto()
    THREE = auto()
    TWO_PAIR = auto()
    ONE_PAIR = auto()
    HIGH_CARD = auto()

order = ["A", "K", "Q", "J", "T", "9", "8", "7", "6", "5", "4", "3", "2" ]
order2 = ["A", "K", "Q", "T", "9", "8", "7", "6", "5", "4", "3", "2", "J" ]


def get_hand_type(hand : str) -> HandType:
    cards = []
    for ch in hand:
        cards.append(ch)
    hand = sorted(cards, key=lambda x: order.index(x[0]))
    hand = "".join(hand)
    if hand[0] == hand[1] == hand[2] == hand[3] == hand[4]:
        return HandType.FIVE
    if hand[0] == hand[1] == hand[2] == hand[3] or hand[1] == hand[2] == hand[3] == hand[4]:
        return HandType.FOUR
    if hand[0] == hand[1] == hand[2] and hand[3] == hand[4]:
        return HandType.FULL_HOUSE
    if hand[0] == hand[1] and hand[2] == hand[3] == hand[4]:
        return HandType.FULL_HOUSE
    if hand[0] == hand[1] == hand[2] or hand[1] == hand[2] == hand[3] or hand[2] == hand[3] == hand[4]:
        return HandType.THREE
    if hand[0] == hand[1] and (hand[2] == hand[3] or hand[3] == hand[4]):
        return HandType.TWO_PAIR
    if hand[1] == hand[2] and hand[3] == hand[4]:
        return HandType.TWO_PAIR
    if hand[0] == hand[1] or hand[1] == hand[2] or hand[2] == hand[3] or hand[3] == hand[4]:
        return HandType.ONE_PAIR
    return HandType.HIGH_CARD

def get_hand_type2_replace_jokers(hand : str, index : int) -> HandType:
    if index == len(hand):
        return get_hand_type(hand)
    if hand[index] != "J":
        return get_hand_type2_replace_jokers(hand, index+1)
    best_hand = HandType.HIGH_CARD
    for ch in order2:
        hand = hand[:index] + ch + hand[index+1:]
        result = get_hand_type2_replace_jokers(hand, index+1)
        if result.value < best_hand.value:
            best_hand = result
        if best_hand == HandType.FIVE:
            return best_hand
    return best_hand

def get_hand_type2(hand : str) -> HandType:
    return get_hand_type2_replace_jokers(hand, 0)

def compare_hands(x : str, y : str) -> int:
    x_hand = get_hand_type(x)
    y_hand = get_hand_type(y)
    if x_hand != y_hand:
        return x_hand.value - y_hand.value
    for i in range(0, len(x)):
        if x[i] != y[i]:
            return order.index(x[i][0]) - order.index(y[i][0])
    return 0

def compare_hands_bids(x : (str,int), y : (str,int)) -> int:
    return compare_hands(x[0], y[0])

def compare_hands_bids_best_type(x : (str,int, HandType), y : (str,int, HandType)) -> int:
    x_hand = x[2]
    y_hand = y[2]
    if x_hand != y_hand:
        return x_hand.value - y_hand.value
    for i in range(0, len(x[0])):
        cx = x[0][i]
        cy = y[0][i]
        if cx != cy:
            return order2.index(cx) - order2.index(cy)
    return 0

def part_a(lines : [str]) -> int:
    hands_bids = []
    for line in lines:
        line = line.split(" ")
        hands_bids.append((line[0], int(line[1])))

    hands_bids = sorted(hands_bids, key= cmp_to_key(compare_hands_bids), reverse=True)
    total = 0
    for i in range(0, len(hands_bids)):
        total += hands_bids[i][1] * (i+1)
    return total

def part_b(lines : [str]) -> int:
    hands_bids = []
    for line in lines:
        line = line.split(" ")
        hands_bids.append((line[0], int(line[1])))

    hands_bids_best_type = [(hand[0], hand[1], get_hand_type2(hand[0])) for hand in hands_bids]

    hands_bids_best_type = sorted(hands_bids_best_type, key= cmp_to_key(compare_hands_bids_best_type), reverse=True)
    total = 0
    for i in range(0, len(hands_bids_best_type)):
        total += hands_bids_best_type[i][1] * (i+1)
    return total


def main():
    lines = []
    with open("day07.txt") as file:
        for line in file:
            line = line.strip()
            lines.append(line)
    print(f"Part A: {part_a(lines)}")   
    print(f"Part B: {part_b(lines)}") 
    
if __name__ == "__main__":
    main()