const Util = {
    deepCopy(obj) {
        if (typeof obj !== 'object' || obj === null) {
            return obj;
        }
    
        let newObj = Array.isArray(obj) ? [] : {};
    
        for (let key in obj) {
            if (Object.prototype.hasOwnProperty.call(obj, key)) {
                newObj[key] = deepCopy(obj[key]);
            }
        }
    
        return newObj;
    }
}