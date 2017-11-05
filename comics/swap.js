<script type="text/javascript">
function swapReprint(){
    var r  = document.getElementById('issuetable').rows
    var l = r.length
    var x
    var temp
    for (x = 1; x < l; x++){ //  skip row 0 cuz it's col headers
        if(r[x].cells.length > 4){
    	    if(r[x].cells[4].innerHTML != "&nbsp;" 
	    && r[x].cells[1].innerHTML != "(tpb/gn)"){
                temp = r[x].cells[4].innerHTML
                r[x].cells[4].innerHTML = 
	               r[x].cells[0].innerHTML + " " + r[x].cells[1].innerHTML
                //  break temp into issue title and number
                var tl = temp.length
	        var ch = temp.charAt(tl - 1)
		while(ch == " "){
                    tl--
	            ch = temp.charAt(tl - 1)
                }
	        if(ch < "0" || ch > "9"){
	            r[x].cells[0].innerHTML = temp 
	            r[x].cells[1].innerHTML = "&nbsp;" 
	        }
	        else{
                    var ishnum = ""
		    var n = tl - 1
		    while(temp.charAt(n) == " "){
                         n--
                    }
		    while(temp.charAt(n) != " "){
		        c = temp.charAt(n)
		        ishnum = c + ishnum
                        n--
		        //alert("ishnum = " + ishnum + "  n = " + n)
		    }
	            r[x].cells[0].innerHTML = temp.substr(0, n) 
	            r[x].cells[1].innerHTML = ishnum   
	        }
	    }
        }
    }
    temp = r[5].cells[0].innerHTML
}
</script>
				
