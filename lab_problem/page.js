const INT_MIN=Number.MIN_VALUE;
const INT_MAX=Number.MAX_VALUE;
function fifo(f,rs) {
    //queue<int> q;
    var q=[];
    //vector<vector<int>> v(f,vector<int> (rs.size(),-1));
    //const INT_MIN=-1000000;
    //const INT_MAX=10000000;
    
    //console.log(obj.length);
    var v=[]
    for(let i=0;i<f;i++){
        v.push([]);
        for(let j=0;j<rs.length;j++){
            v[i].push(-1);

        }
    }
   
    let miss =0;
    let cycle = 0;
    for (let i =0;i<rs.length;i++) {
        let page = rs[i];
        let find=false;
        let emp = false;
        if (i != 0) {
            for (let j =0;j<f;j++) {
                v[j][i] = v[j][i-1];
            }
        }
        for (let j =0;j<f;j++) {
            if (v[j][i] !=-1 && v[j][i] == page) {
                find =true;
                break;
            } else if (v[j][i] == -1){
                emp = true;
            }
        }
        if (!find) {
            q.push(page);
            process.stdout.write("0 ");
            miss++;
            if (emp) {
                v[cycle][i] = page;
                cycle = (cycle + 1)%f;
            } else {
                let fif = q[0];
                q.shift();
                for (let ii =0;ii<f;ii++) {
                    if (v[ii][i] == fif) {
                        v[ii][i] = page; 
                        break;
                    }
                }
            }
            //replace page
        }
        else {
            process.stdout.write("1 ");
        } 
    }
    return miss;

}

function lru(f,rs){
    //const INT_MIN=-1000000;
    //const INT_MAX=10000000;
    var v=[]
    for(let i=0;i<f;i++){
        v.push([]);
        for(let j=0;j<rs.length;j++){
            v[i].push(-1);

        }
    }
    //console.log(v[0][0]);
    //console.log(v[0].length);
    var miss=0;
    var cycle =0;
    //console.log(rs.length);
    for(let i=0;i<rs.length;i=i+1){
        //console.log(i);
        
        let page=rs[i];
        //console.log(page);
        let find=0;
        let emp=0;
        if(i!=0){
            for(let j=0;j<f;j++){
                v[j][i]=v[j][i-1];
            }
        }
        // for(let j=0;j<f;j++){
        //     process.stdout.write(v[j][i]+" ");
        // }
        // console.log("");
        for (let j =0;j<f;j++) {
           
            if (v[j][i] !=-1 && v[j][i] == page) {
                find =1;
                break;
            } else if (v[j][i] == -1){
                emp = 1;
            }
        }
        if (!find) {
            //console.log("0 ");
            process.stdout.write("0 ");
            miss=miss+1;
            if (emp) {
                //console.log("cycle" +cycle);
                v[cycle][i] = page;
                cycle = (cycle + 1)%f;
                
            } else {
                let imd = -1;
                let maxdis = INT_MIN;
                for (let m = 0;m<f;m++) {
                    let dis = 0;
                    let b = false;
                    for (let p = i-1;p>-1;p--) {
                        let val = rs[p];
                        dis++;
                        if (val == v[m][i]) {
                            b=true;
                            break;
                        }
                    }
                    if (!b) {
                        dis = INT_MAX;
                    }
                    if (dis > maxdis) {
                        maxdis = dis;
                        imd = m;
                    }
                }
                v[imd][i] = page;
            }
            //replace page
        }
        else {
            //console.log("1 ");
            process.stdout.write("1 ");
        } 
       
        
      
    }
    return miss;
}
function opr(f,rs){
    var v=[]
    for(let i=0;i<f;i++){
        v.push([]);
        for(let j=0;j<rs.length;j++){
            v[i].push(-1);

        }
    }
    
    let miss =0;
    let cycle = 0;
    for (let i =0;i<rs.length;i++) {
        let page = rs[i];
        let find=false;
        let emp = false;
        if (i != 0) {
            for (let j =0;j<f;j++) {
                v[j][i] = v[j][i-1];
            }
        }
        for (let j =0;j<f;j++) {
            if (v[j][i] !=-1 && v[j][i] == page) {
                find =true;
                break;
            } else if (v[j][i] == -1){
                emp = true;
            }
        }
        if (!find) {
            //cout << "0 ";
            process.stdout.write("0 ");
            miss++;
            if (emp) {
               

                v[cycle][i] = page;
                cycle = (cycle + 1)%f;
            } else {
                let imd =-1;

                let maxdis = INT_MIN;
                for (let m = 0;m<f;m++) {
                    let dis = 0;
                    let b = false;
                    for (let p = i+1;p<rs.length;p++) {
                        let val = rs[p];
                        dis++;
                        if (val == v[m][i]) {
                            b=true;
                            break;
                        }
                    }
                    if (!b) {
                        dis = INT_MAX;
                    }
                    if (dis > maxdis) {
                        maxdis = dis;
                        imd = m;
                    }
                }
                v[imd][i] = page;                
            }
            //replace page
        }
        else {
            process.stdout.write("1 ");
        } 
    }
    return miss;
}
let g=lru(2,[1,2,2,4,5,5,3,2,1]);
console.log(g);
g=fifo(2,[1,2,2,4,5,5,3,2,1]);
console.log(g);
g=opr(2,[1,2,2,4,5,5,3,2,1]);
console.log(g);
