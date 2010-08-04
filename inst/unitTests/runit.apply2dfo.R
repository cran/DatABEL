### --- Test setup ---

if(FALSE) {
	## Not really needed, but can be handy when writing tests
	library("RUnit")
	library("DatABEL")
}

#test.empty <- function(){}
### do not run
#stop("SKIP THIS TEST")
###

### ---- common functions and data -----

#source("../inst/unitTests/shared_functions.R")
source(paste(path,"/shared_functions.R",sep=""))

### --- Test functions ---

test.apply2dfo <- function()
{
#    library("RUnit")
#    library("DatABEL")
#    source("shared_functions.R")

    unlink("tmp*")

#	testmatr <- make_random_matrix(range_dim1=c(10,100),range_dim2=c(10,100),range_data = c(-10,10))
	testmatr <- make_matrix_of_type_with_NA(na.proportion=0)
	testmatr[sample(1:length(testmatr),min(10,floor(length(testmatr)*0.01)))] <- NA
	dfo <- as(testmatr,"databel")

    res0 <- apply(testmatr,MAR=2,FUN=sum)
    print("res0")
    print(res0)
    pfun <- function(a) return(a)
    res1 <- apply2dfo(SNP,anFUN="sum",MAR=2,procFUN="pfun",dfodata=dfo,transpose=F)
    print(res1)
    res2 <- apply2dfo(SNP,anFUN="sum",MAR=2,procFUN="pfun",dfodata=dfo,transpose=F,outclass="databel",outfile="tmp")
    print(res2)
    res3 <- apply2dfo(SNP,anFUN="sum",MAR=2,procFUN="pfun",dfodata=dfo,transpose=T)
    print(res3)
    res4 <- apply2dfo(SNP,anFUN="sum",MAR=2,procFUN="pfun",dfodata=dfo,transpose=T,outclass="databel",outfile="tmp1")
    print(res4)
    checkEqualsNumeric(as(res0,"vector"),as(res1,"vector"),tol=5*sqrt(.Machine$double.eps))
    checkEqualsNumeric(as(res0,"vector"),as(res2,"vector"),tol=5*sqrt(.Machine$double.eps))
    checkEqualsNumeric(as(res0,"vector"),as(res3,"vector"),tol=5*sqrt(.Machine$double.eps))
    checkEqualsNumeric(as(res0,"vector"),as(res4,"vector"),tol=5*sqrt(.Machine$double.eps))
    rm(res1,res2,res3,res4)
    gc()
    
    res0 <- apply(testmatr,MAR=1,FUN=sum)
    print("res0-1")
    print(res0)
    pfun <- function(a) return(a)
    res1 <- apply2dfo(SNP,anFUN="sum",MAR=1,procFUN="pfun",dfodata=dfo,transpose=F)
    print(res1)
    res2 <- apply2dfo(SNP,anFUN="sum",MAR=1,procFUN="pfun",dfodata=dfo,transpose=F,outclass="databel",outfile="tmp2")
    print(res2)
    res3 <- apply2dfo(SNP,anFUN="sum",MAR=1,procFUN="pfun",dfodata=dfo,transpose=T)
    print(res3)
    res4 <- apply2dfo(SNP,anFUN="sum",MAR=1,procFUN="pfun",dfodata=dfo,transpose=T,outclass="databel",outfile="tmp3")
    print(res4)
    checkEqualsNumeric(as(res0,"vector"),as(res1,"vector"),tol=5*sqrt(.Machine$double.eps))
    checkEqualsNumeric(as(res0,"vector"),as(res2,"vector"),tol=5*sqrt(.Machine$double.eps))
    checkEqualsNumeric(as(res0,"vector"),as(res3,"vector"),tol=5*sqrt(.Machine$double.eps))
    checkEqualsNumeric(as(res0,"vector"),as(res4,"vector"),tol=5*sqrt(.Machine$double.eps))
    rm(list=ls())
    gc()

    unlink("tmp*")

}