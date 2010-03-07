\name{databel_filtered_R-class}
\Rdversion{1.1}
\docType{class}
\alias{databel_filtered_R-class}
\alias{[,databel_filtered_R-method}
\alias{[<-,databel_filtered_R-method}
\alias{connect,databel_filtered_R-method}
\alias{dim,databel_filtered_R-method}
\alias{dimnames,databel_filtered_R-method}
\alias{dimnames<-,databel_filtered_R-method}
\alias{length,databel_filtered_R-method}
\alias{save_as,databel_filtered_R-method}

\title{Class "databel_filtered_R"}
\description{
A class interfacing FILEVECTOR C++ library 
for storage of and fast consecutive access to 
large data matrices in out-of-RAM disk mode 
with regulated cache size. Columns of matrix 
are accessible very quickly.
The 'filtered' 
class is based on 'base' class, see 
methods for that class for more details. 
}
\section{Objects from the Class}{
Objects can be created by calls of the form \code{new("databel_filtered_R", baseobject)}.
FILEVECTOR data are stored using files of form BASE.fvi (index) and BASE.fvd (data). 
"baseobject" is either the BASE name, or object of class \code{"\linkS4class{databel_base_R}"}.
}

\section{Slots}{
  \describe{
    \item{\code{usedRowIndex}:}{Object of class \code{"integer"} which rows are used }
    \item{\code{usedColIndex}:}{Object of class \code{"integer"} which columns are used }
    \item{\code{unique.names}:}{Object of class \code{"logical"} if all dimnames are unique }
    \item{\code{unique.colnames}:}{Object of class \code{"logical"} if column names are unique }
    \item{\code{unique.rownames}:}{Object of class \code{"logical"} if row names are unique }
    \item{\code{backingfilename}:}{Object of class \code{"character"} providing BASE name}
    \item{\code{cachesizeMb}:}{Object of class \code{"integer"} size of cache to be used to 
    access the data. If cache is equal to the data size, the object is stored in RAM}
    \item{\code{data}:}{Object of class \code{"externalptr"}, pointer to AbstractFileVector 
    C++ object}
  }
}

\section{Extends}{
Class \code{"\linkS4class{databel_base_R}"}, directly, with explicit coerce.
}

\section{Methods}{
  \describe{
    \item{[}{\code{signature(x = "databel_filtered_R")}: sub-setting object }
    \item{[<-}{\code{signature(x = "databel_filtered_R")}: setting the values in the object }
    \item{connect}{\code{signature(object = "databel_filtered_R")}: connects the data files to R object
    (calls constructor of AbstractFileVeector object, selects rows and columns) }
    \item{dim}{\code{signature(x = "databel_filtered_R")}: returns dimensions of the matrix }
    \item{dimnames}{\code{signature(x = "databel_filtered_R")}: returns row and column names }
    \item{dimnames<-}{\code{signature(x = "databel_filtered_R")}: sets row and column names }
    \item{length}{\code{signature(x = "databel_filtered_R")}: returns number of elements in the matrix }
    \item{save_as}{\code{signature(x = "databel_filtered_R")}: saves (a sub-set of) the object }    
	 }
}

\references{\url{http://mga.bionet.nsc.ru/~yurii/ABEL/}}

\author{Yurii Aulchenko}

%\note{ ~~further notes~~ }

% ~Make other sections like Warning with \section{Warning }{....} ~

\seealso{
	\code{\link{make_empty_fvf}},
	\code{\link{matrix2databel_base_R}},
	\code{\linkS4class{databel_base_R-class}}
}

\examples{
showClass("databel_filtered_R")
}
\keyword{classes}
