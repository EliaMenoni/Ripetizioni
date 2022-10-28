function deep(root) {
    
}

var T =
{
    val: 5,
    sx: {
        val: 7,
        sx: { val: 'c' },
        dx: {
            val: 5,
            sx: { val: 10 }
        }
    },
    dx: {
        val: 'a',
        sx: {
            val: 'b',
            dx: { val: 7 }
        },
        dx: { val: 1 }
    }
}

console.log(deep(T)[1])
