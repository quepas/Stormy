go <- local({

  get_request <- function() {
    request <- readline("\tRequest: ")
  }

  fetch_data <- function(address, request) {
    full_host <- paste(address$host, address$port, sep=":")
    getURL(paste(full_host, request, sep="/"))
  }

  get_remote_server <- function() {
    message("\nEnter Stormy Remote Server address:")
    host <- readline("\tHost: ")
    port <- readline("\tPort: ")
    list(host = host, port = port)
  }

  check_packages <- function() {
    all <- installed.packages()
    required <- c("RCurl")
    needed <- !(required %in% rownames(all))

    if(any(needed)) {
      message("\nSome packages are missing.\n")
      message("Install ", paste(required[needed], collapse=", "))
    }
  }

  function() {
    check_packages()
    address <- get_remote_server()

    repeat {
      request <- get_request()
      message(fetch_data(address, request))
    }
  }
})