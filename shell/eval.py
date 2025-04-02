import ast
import sys
import operator

# Allowed operators
allowed_operators = {
    ast.Add: operator.add,
    ast.Sub: operator.sub,
    ast.Mult: operator.mul,
    ast.Div: operator.truediv,
    ast.Pow: operator.pow,
    ast.Mod: operator.mod,
    ast.UAdd: operator.pos,
    ast.USub: operator.neg
}

def safe_eval(node):
    """Evaluate an AST node safely."""
    if isinstance(node, ast.Num):  # Check if it's a number
        return node.n
    elif isinstance(node, ast.BinOp):  # Check if it's a binary operation
        left = safe_eval(node.left)
        right = safe_eval(node.right)
        if type(node.op) in allowed_operators:
            return allowed_operators[type(node.op)](left, right)
    elif isinstance(node, ast.UnaryOp):  # Check if it's a unary operation
        operand = safe_eval(node.operand)
        if type(node.op) in allowed_operators:
            return allowed_operators[type(node.op)](operand)
    else:
        raise TypeError("Unsupported operation")

def calculate(expression):
    """Calculate the value of the expression."""
    parsed_expr = ast.parse(expression, mode='eval')
    return safe_eval(parsed_expr.body)

def main():
    # Check command line arguments
    if len(sys.argv) < 2:
        print("Error: No expression provided.")
        sys.exit(1)

    # Try to calculate
    try:
        expression = sys.argv[1]
        result = calculate(expression)
        print(result)
    except Exception as e:
        print(f"Error calculating expression: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
