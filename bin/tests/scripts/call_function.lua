
function AddStuff(a, b)
    print("[LUA] AddStuff("..a..", "..b..") called \n")
    return a + b
end

function DoAThing(a, b)
    print("[LUA] DoAThing("..a..", "..b..") called \n")
    c = HostFunction(a + 10, b * 3)
    c = HostLambda(c)
    return c
end