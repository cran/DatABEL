\name{databel_base_R-class}

\Rdversion{1.1}
\docType{class}

\alias{databel_base_R-class}
\alias{[,databel_base_R-method}
\alias{[<-,databel_base_R-method}
\alias{backingfilename,databel_base_R-method}
\alias{backingfilename}
\alias{cachesizeMb,databel_base_R-method}
\alias{cachesizeMb}
\alias{cachesizeMb<-,databel_base_R-method}
\alias{cachesizeMb<-}
\alias{connect,databel_base_R-method}
\alias{connect}
\alias{dim,databel_base_R-method}
\alias{dimnames,databel_base_R-method}
\alias{disconnect,databel_base_R-method}
\alias{disconnect}
\alias{get_dimnames,databel_base_R-method}
\alias{get_dimnames}
\alias{length,databel_base_R-method}
\alias{save_as,databel_base_R-method}
\alias{save_as}
\alias{set_dimnames<-,databel_base_R-method}
\alias{set_dimnames<-}

\title{Class "databel_base_R"}

\description{
A class interfacing FILEVECTOR C++ library 
for storage of and fast consecutive access to 
large data matrices in out-of-RAM disk mode 
with regulated cache size. Columns of matrix 
are accessible very quickly. 
}

\section{Objects from the Class}{
Objects can be created by calls of the form \code{new("databel_base_R", filename, cachesizeMb)}.
FILEVECTOR data are stored using files of form BASE.fvi (index) and BASE.fvd (data). 
"filename" is the BASE name.  
}

\section{Slots}{
  \describe{
    \item{\code{backingfilename}:}{Object of class \code{"character"} providing BASE name}
    \item{\code{cachesizeMb}:}{Object of class \code{"integer"} size of cache to be used to 
    access the data. If cache is equal to the data size, the object is stored in RAM}
    \item{\code{data}:}{Object of class \code{"externalptr"}, pointer to AbstractFileVector 
    C++ object}
  }
}

\section{Methods}{
  \describe{
    \item{[}{\code{signature(x = "databel_base_R")}: sub-setting object}
    \item{[<-}{\code{signature(x = "databel_base_R")}: setting the values in the object }
    \item{backingfilename}{\code{signature(object = "databel_base_R")}: returns BASE FILEVECTOR file 
    name used to store the data}
    \item{cachesizeMb}{\code{signature(object = "databel_base_R")}: returns the cache size used}
    \item{cachesizeMb<-}{\code{signature(x = "databel_base_R")}: sets new cache size}
    \item{connect}{\code{signature(object = "databel_base_R")}: connects the data files to R object
    (calls constructor of AbstractFileVeector object)}
    \item{dim}{\code{signature(x = "databel_base_R")}: returns dimensions of the matrix}
    \item{dimnames}{\code{signature(x = "databel_base_R")}: dummy, returns NULL }
    \item{disconnect}{\code{signature(object = "databel_base_R")}: disconnects the R object from data files
    (calls destructor of AbstractFileVeector object)}
    \item{get_dimnames}{\code{signature(object = "databel_base_R")}: returns the names of rows and columns,
    which may be non-unique }
    \item{length}{\code{signature(x = "databel_base_R")}: returns number of elements in the matrix }
    \item{save_as}{\code{signature(x = "databel_base_R")}: saves (a sub-set of) the object }
    \item{set_dimnames<-}{\code{signature(x = "databel_base_R")}: set row and column names, 
    which may be non-unique }
	 }
}
\references{\url{http://mga.bionet.nsc.ru/~yurii/ABEL/}}

\author{Yurii Aulchenko}

% \note{}

\seealso{
	\code{\link{make_empty_fvf}},
	\code{\linkS4class{databel_filtered_R-class}}
}

\examples{
showClass("databel_base_R")
}

\keyword{classes}
