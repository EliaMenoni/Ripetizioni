function tree_def(depth, figli, root = []) {
    if (depth == 1) return root["Val"] = Math.floor(Math.random() * 10);

    root["Val"] = Math.floor(Math.random() * 10);

    for (let i = 0; i < figli; i++) {
        let node = tree_def(depth - 1, figli);
        if (node != null) root["Leaf" + (i + 1)] = node;
    }
    return root
}

let tree = tree_def(5,2);
console.log(tree);

