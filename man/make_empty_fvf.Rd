\name{make_empty_fvf}
\alias{make_empty_fvf}
\title{makes empty filevector object}
\usage{
  make_empty_fvf(name, nvariables, nobservations,
    type = "DOUBLE", cachesizeMb = 64, readonly = FALSE)
}
\arguments{
  \item{name}{name fo the file to be assoiated with new
  object}

  \item{nvariables}{number of variables (R columns)}

  \item{nobservations}{number of observations (R rows)}

  \item{type}{data type of the object
  ("UNSIGNED_SHORT_INT", "SHORT_INT", "UNSIGNED_INT",
  "INT", "FLOAT", "DOUBLE", "CHAR", "UNSIGNED_CHAR")}

  \item{cachesizeMb}{what cache size to use for newly
  generated 'databel' object}

  \item{readonly}{whether to open new 'databel' in readonly
  mode}
}
\value{
  databel object; also file is created in file system
}
\description{
  function to generate empty filevector object (and disk
  files)
}

