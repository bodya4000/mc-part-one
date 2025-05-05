#include <Arduino.h>
#include "HtmlPageBuilder.h"

String HtmlPageBuilder::home() {
    return R"rawliteral(
        <!DOCTYPE html>
					<html>
						<head>
							<title>LAB_1</title>
						</head>
						<body style="text-align: center; padding-top: 50px">
							<button id="Button" style="font-size: 30px; padding: 20px 40px">Hold Me</button>

							<script>
								let holdTimeout;
								const btn = document.getElementById('Button');

								btn.addEventListener('mousedown', () => {
									holdTimeout = setTimeout(() => {
										fetch('/press', { method: 'POST' });
									}, 1000);
								});

								btn.addEventListener('mouseup', () => {
									clearTimeout(holdTimeout);
									fetch('/press_out', { method: 'POST' });
								});
							</script>
						</body>
					</html>

    )rawliteral";
}

String HtmlPageBuilder::notFound() {
    return "<h1>404 - Not found</h1>";
}
