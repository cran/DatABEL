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

source(paste(path,"/shared_functions.R",sep=""))

### --- Test functions ---

# make matrix, convert to fv, back and compare
test.NA_test <- function(){
  unlink("*.fv*");
  unlink("tmp*")
  
  v <- make_random_matrix();
  whichNA <- sample(1:length(v),round(length(v)*0.1))
  v[whichNA] <- NA
  f <- matrix2databel(v,"tmp",64,"DOUBLE");
  v1 <- as(f,"matrix")
  checkEquals(v,v1);
  
  rm(f);gc()
  unlink("*.fv*");
  unlink("tmp*")
  
}

test.NA_export_import <- function(){
  unlink("*.fv*");
  unlink("tmp*")
  
  v=matrix(c(1,NA,2.000123456,3),2,2);
  f=matrix2databel(v,"tmp",64,"INT");
  save_as_text(f,c(1,2),c(1,2),"tmp2");
  f2 = text2filevector("tmp2","tmp3",c(1,2),c(1,2),0,0,
      FALSE,R_matrix=FALSE,type="INT",readonly=TRUE, naString="NA");
  m2=as(f2,"matrix");
  checkEquals(m2[1,1],1);
  checkTrue(is.na(m2[2,1]));
  checkEquals(m2[1,2],2.);
  checkEquals(m2[2,2],3.);
  
  rm(f,f2);gc()
  unlink("*.fv*");
  unlink("tmp*");
}

test.NA_string <- function(){
  unlink("*.fv*");
  unlink("tmp*")
  
  v=matrix(c(1,2,4,123),2,2);
  f=matrix2databel(v,"tmp",64,"INT");
  save_as_text(f,c(1,2),c(1,2),"tmp2");
  f2 = text2filevector("tmp2","tmp3",c(1,2),c(1,2),0,0,
      FALSE,R_matrix=FALSE,type="INT",readonly=TRUE, naString="123");
  v2 <- v
  v2[v2=="123"] <- NA
  m2=as(f2,"matrix");
  checkEquals(m2,v2)
  checkEquals(m2[1,1],1);
  checkEquals(m2[2,1],2);
  checkEquals(m2[1,2],4);
  checkTrue(is.na(m2[2,2]));
  
  rm(f,f2);gc()
  unlink("*.fv*");
  unlink("tmp*")
}

