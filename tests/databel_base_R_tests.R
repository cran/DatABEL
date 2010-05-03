library(DatABEL)
source("test_funs.R")

system("rm test*fv* tmp*fv*")

NVAR <- 20
NOBS <- 10

# generate test data
dta <- matrix(rnorm(NVAR*NOBS),ncol=NVAR,nrow=NOBS)
colnames(dta) <- paste("col",1:NVAR,sep="")
rownames(dta) <- paste("row",1:NOBS,sep="")
dim(dta)

# create databel based on test data in different ways and test
x <- as(dta,"databel")
class(x)
testobject(dta,as(x,"matrix"))
disconnect(x)
connect(x)
class(x)
testobject(dta,as(x,"matrix"))

x <- matrix2databel(dta,file="test.fvf",cachesizeMb=128)
class(x)
testobject(dta,as(x,"matrix"))

x <- as(dta,"databel")
class(x)
testobject(dta,as(x,"matrix"))

rm(x)
gc()
system("rm tmp*fv* test*fv*")

#

x <- matrix2databel(dta,file="test.fvf",cachesizeMb=128)
class(dta)
class(x)
dta
x
rm(x)
gc()

# load object
x <- databel("test.fvf",64,readonly=FALSE)
x
table(abs(as(x,"matrix")-dta)>1e-7)
testobject(dta,x)
testobject(dta,as(x,"matrix"))
testobject(dta,x,FUN=as.vector)

# ![      keeping class
#x[1:3,1:5]
#class(x[1:3,1:5])

# [<-    keeping class
# fails here
# setReadOnly(x) <- FALSE
x[2,] <- rnorm(ncol(x))+100
x[,3] <- rnorm(nrow(x))+100
class(x)
x
x[]
x[,1]

# dim
testobject(dta,x,dim)
testobject(dta,x,ncol)
testobject(dta,x,nrow)
dim(x)
ncol(x)
nrow(x)
# dimnames  -- fake, return NULL
testobject(dta,x,dimnames)
testobject(dta,x,colnames)
testobject(dta,x,rownames)
dimnames(x)
colnames(x)
rownames(x)

# show
x
#
# !cbind
# !rbind
#
# is.databel
is(x,"databel")
is(c(0,1,2),"databel")

# as.databel -- not real yet
tmp <- matrix(rnorm(5*7),5,7)
tmp1 <- as(tmp,"databel")

# as.matrix
class(x)
as(x,"matrix")[1:7,1:3]
class(as(x,"matrix"))

#
# backingfilename
backingfilename(x)

# get_cachesizeMb
cachesizeMb(x)

# set_cachesizeMb -- not real yet
cachesizeMb(x) <- 128

# get_dimnames
a <- get_dimnames(x)
class(a)
a

# connect
connect(x)
x
# disconnect
x
disconnect(x)
x@data
x

# connect
connect(x,readonly=FALSE)
x

#
# dealing with NAs
#

a <- rnorm(ncol(x))
a[c(2,5)] <- NA
a
mean(a)
mean(a,na.rm=T)
x[2,] <- a
disconnect(x)
connect(x)
x
mean(x[2,])
mean(x[2,],na.rm=T)

#q() 

x[1:10,1:5]
class(x)
herex <- x[]
class(herex)
dim(herex)

system("rm test*fv* tmp*fv*")


