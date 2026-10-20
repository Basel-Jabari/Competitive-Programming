"""Compatibility entry point: restored tree contracts are now in restored_checks.py."""
from restored_checks import BUILD, MAPPING, compile_one, tree_checks

if __name__ == "__main__":
    BUILD.mkdir(parents=True, exist_ok=True)
    for item in MAPPING:
        if item[2] in {"13", "14", "15", "16", "43"}:
            compile_one(item)
    tree_checks()
