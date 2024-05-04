if(degree[seq][1] > 1) {
    entry = seq;                // 标记当前并发单元入口
    if(pair.find(entry) == null) {
        pair[entry] = -1;      // 保存key为当前并发入口
        stack.push(entry);     // 将entry压入匹配栈
    }
}
if(degree[seq][0] > 1) {
    exit = seq;                 // 标记当前并发出口
    curEntry = stk.top();       // 取出栈顶并发入口
    if(pair.find(curEntry) == null) // 之前未出现并发入口
    {
        return false;          // 并发出口先于并发入口出现
    }
    else {
        if(pair[curEntry] == -1) // 匹配栈顶取出的并发入口尚未匹配
        {
            pair[curEntry] = exit;  // 保存value为该并发出口
            stack.pop();            // 完成匹配，弹出匹配栈
            if(matrix[curEntry][exit] == 1 ||
                matrix[exit][curEntry] == 1)
            {
                return false;   // 并发出口与并发入口直接相连
            }
        }
        else if(pair[curEntry] != exit)
        {
            return false;       // 并发入口curEntry存在多个并发出口
        }
    }
}

for( i : nextNodes(seq) )   // 查询seq所指向的下一层节点
{
    if(path[seq][i] == 0)
    {
        传入i作为seq，递归调用
    }
}
