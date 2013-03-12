\name{databel-class}
\Rdversion{1.1}
\docType{class}
\alias{databel-class}
\alias{[-methods}
\alias{[,databel,ANY,ANY,ANY-method}
\alias{[,databel-method}
\alias{[<-,databel-method}
\alias{connect,databel-method}
\alias{connect}
\alias{disconnect,databel-method}
\alias{disconnect}
\alias{dim,databel-method}
\alias{dimnames,databel-method}
\alias{dimnames<-,databel-method}
\alias{set_dimnames<-,databel-method}
\alias{set_dimnames<-}
\alias{length,databel-method}
\alias{save_as,databel-method}
\alias{save_as}
\alias{save_as_text,databel-method}
\alias{save_as_text}
\alias{backingfilename,databel-method}
\alias{backingfilename}
\alias{cachesizeMb,databel-method}
\alias{cachesizeMb}
\alias{cachesizeMb<-,databel-method}
\alias{cachesizeMb<-}
\alias{get_dimnames,databel-method}
\alias{get_dimnames}
\alias{set_dimnames<-,databel-method}
\alias{set_dimnames<-}
\alias{setReadOnly<-,databel-method}
\alias{setReadOnly<-}


\title{Class "databel"}
\description{
A class interfacing FILEVECTOR C++ library
class FilteredMatrix  
for storage of and fast consecutive access to 
large data matrices in out-of-RAM disk mode 
with regulated cache size. Columns of matrix 
are accessible very quickly.
}
\section{Objects from the Class}{
Objects can be created by calls of the form \code{new("databel", baseobject)}
or \code{databel(baseobject)}.
FILEVECTOR data are stored using files of form BASE.fvi (index) and BASE.fvd (data). 
"baseobject" is either the BASE name, or object of class \code{"\linkS4class{databel}"}.
}

\section{Slots}{
  \describe{
    \item{\code{usedRowIndex}:}{Object of class \code{"integer"} which rows are used }
    \item{\code{usedColIndex}:}{Object of class \code{"integer"} which columns are used }
    \item{\code{uninames}:}{Object of class \code{"list"}, containing obejcts 'unique.names' 
    -- TRUE if all dimnames are unique; 'unique.colnames' -- if column names are unique;
    'unique.rownames' -- if row names are unique }
    \item{\code{backingfilename}:}{Object of class \code{"character"} providing BASE name}
    \item{\code{cachesizeMb}:}{Object of class \code{"integer"} size of cache to be used to 
    access the data. If cache is equal to the data size, the object is stored in RAM}
    \item{\code{data}:}{Object of class \code{"externalptr"}, pointer to FilteredMatrix 
    C++ object}
  }
}

%\section{Extends}{
%}

\section{Methods}{
  \describe{
    \item{[}{\code{signature(x = "databel")}: sub-setting object }
    \item{[<-}{\code{signature(x = "databel")}: setting the values in the object }
    \item{connect}{\code{signature(object = "databel")}: connects the data files to R object
    (calls constructor of FilteredMatrix object, selects rows and columns) }
    \item{disconnect}{\code{signature(object = "databel")}: disconnects the data files to R object
    (calls destructor of FilteredMatrix object, selects rows and columns) }
    \item{dim}{\code{signature(x = "databel")}: returns dimensions of the matrix }
    \item{dimnames}{\code{signature(x = "databel")}: returns row and column names }
    \item{dimnames<-}{\code{signature(x = "databel")}: sets row and column names }
    \item{set_dimnames<-}{\code{signature(x = "databel")}: sets row and column names (these could be non-unique) }
    \item{length}{\code{signature(x = "databel")}: returns number of elements in the matrix }
    \item{save_as}{\code{signature(x = "databel")}: saves (a sub-set of) the object as FV-file }   
    \item{save_as_text}{\code{signature(x = "databel")}: saves (a sub-set of) the object as plain text file }   
    \item{backingfilename}{\code{signature(object = "databel")}: returns BASE FILEVECTOR file 
    name used to store the data}
    \item{cachesizeMb}{\code{signature(object = "databel")}: returns the cache size used}
    \item{cachesizeMb<-}{\code{signature(x = "databel")}: sets new cache size}
    \item{get_dimnames}{\code{signature(object = "databel")}: returns the names of rows and columns,
    which may be non-unique }
    \item{set_dimnames<-}{\code{signature(x = "databel")}: set row and column names, 
    which may be non-unique }
    \item{setReadOnly<-}{\code{signature(x = "databel")}: sets ReadOnly (TRUE or FALSE) 
    attribute to 'databel' object}
    
     
	 }
}

\references{\url{http://www.genabel.org/packages/DatABEL}}

\author{Yurii Aulchenko, Stepan Yakovenko, Andrey Chernyh}

%\note{ ~~further notes~~ }

% ~Make other sections like Warning with \section{Warning }{....} ~

\seealso{
	\code{\link{make_empty_fvf}},
	\code{\link{databel}},
	\code{\link{matrix2databel}}
}

\examples{
showClass("databel")
}
\keyword{classes}
