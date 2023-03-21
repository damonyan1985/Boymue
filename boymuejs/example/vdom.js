/* Example:
   <view>
       <button value="v">hello</button>
   </view>
 
   translate to config json
   {
    "tag": "view",
    "children": [
        {
            "tag": "button",
            "props": {"value": "v"},
            "children": [
                {"tag": "#text", "text": "hello"}
            ]
        }
    ]
   }
*/

const VNodeTagMap = {

}

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
        // 对应C++端的uid
        this.id = ++idGen;
    }

    addChild(child) {
        this.children.push(child)
    }
}

class VDom {
    constructor(xmlConfig, data = {}) {
        this.config = xmlConfig
        this.data = {}
        this.root = this.genVDom()
        this.nodeCreateMap = {
            'view': this._createNodeView,
            'image': this._createNodeImage,
            '#text': this._createNodeText,
            'button': this._createNodeButton,
            'textfield': this._createNodeTextField
        }
    }

    _createNodeView = (node) => {
        // 如果有迭代
        if (node.props.for) {

        }

        if (node.props.if) {

        }
    }

    _createNodeImage = (node) => {

    }

    _createNodeText = (node) => {

    }

    _createNodeButton = (node) => {

    }

    _createNodeTextField = (node) => {

    }

    // 创建渲染树
    createRender() {
        boymue.operation(JSON.stringify({}))
    }

    // 使用配置信息生成dom树
    genVDom() {
        let node = this.config
        this.nodeCreateMap[node.tag](node)
    }

    diffDom(newNode) {
        boymue.operation(JSON.stringify(diff(null, this.root, newNode)));
        this.root = newNode;
    }
}
