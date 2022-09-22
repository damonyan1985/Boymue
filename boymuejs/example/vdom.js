const VCmdType = {
    kAddNode: 1,
    kSetProp: 2,
    kRemoveNode: 3,
    kReplaceNode: 4
}

function addNode(parant, node) {
    let cmds = []

    if (!node) {
        return cmds
    }

    cmds.push({
        type: VCmdType.kAddNode,
        pid: parant.id,
        id: newNode.id,
        tag: newNode.tag
    })
    
    // 添加prop
    Object.keys(node).forEach((key) => {
        cmds.push({
            type: VCmdType.kSetProp,
            id: node.id,
            key: key,
            value: node[key] // string
        })
    })

    for (let i = 0; i < newNode.children.length; i++) {
        cmds = cmds.concat(addNode(newNode, node.children[i]))
    }
    return cmds;
}

function testimport() {
    return "testimport func";
}

// 将cmd命令作为一个数组进行输出
function diff(parant, newNode, oldNode) {
    if (!oldNode) {
        return addNode(parant, newNode)
    } else if (!newNode) {
        return [{
            type: VCmdType.kRemoveChild,
            pid: parant.id,
            id: oldNode.id
        }]
    } else if (newNode.tag != oldNode.tag) {
        return [{
            type: VCmdType.kRemoveNode,
            pid: parant.id,
            id: oldNode.id
        }, ...addNode(parant, newNode)]
    } else {
        const newLength = newNode.children.length;
        const oldLength = oldNode.children.length;
        let cmds = [];
        for (let i = 0; i < newLength || i < oldLength; ++i) {
            cmds.concat(diff(oldNode,
                newNode.children[i],
                oldNode.children[i],
            ));
        }
    }

    return cmds;
}

class VNode {
    static idGen = 0;

    constructor(tag, props, children, key) {
        this.tag = tag
        this.props = props ?? {}
        this.children = children ?? []
        this.key = key
        this.id = ++idGen;
    }

    addChild(child) {
        this.children.push(child)
    }
}

class VDom {
    constructor() {
        this.root = new VNode('view', {}, [], null);
    }

    diffDom(newNode) {
        boymue.operation(JSON.stringify(diff(null, this.root, newNode)));
        this.root = newNode;
    }
}
